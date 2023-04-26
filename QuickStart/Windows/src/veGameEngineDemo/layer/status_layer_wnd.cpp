#include "pch.h"
#include "status_layer_wnd.h"


#define CHECK_LAYER_STATE                       if (should_stop_) { \
                                                    return; \
                                                }

static const wchar_t* g_wnd_class = TEXT("LayerWndClass");
static const wchar_t* g_wnd_name = TEXT("LayerWnd");


LRESULT CALLBACK readyLayerWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

StatusLayerWnd::~StatusLayerWnd() {
    cleanUp();
}

bool StatusLayerWnd::registerWndClass() {
    WNDCLASSEXW wndClass;
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = readyLayerWndProc;
    wndClass.cbClsExtra = sizeof(long);
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = inst_;
    wndClass.hIcon = NULL;
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = g_wnd_class;
    wndClass.hIconSm = NULL;
    RegisterClassExW(&wndClass);

    return true;
}

bool StatusLayerWnd::open(HINSTANCE inst, HWND parent_wnd) {
    if (layer_wnd_) {
        veLOGW("Layer windows is not null");
        return false;
    }
    parent_wnd_ = parent_wnd;
    inst_ = inst;

    if (!registerWndClass()) {
        veLOGW("registerwndclass failed");
        return false;
    }

    WINDOWINFO info = { sizeof(WINDOWINFO) };
    if (!GetWindowInfo(parent_wnd_, &info)) {
        veLOGW("GetWindowInfo failed, error: {}", GetLastError());
        return false;
    }
    int width = info.rcClient.right - info.rcClient.left;
    int height = info.rcClient.bottom - info.rcClient.top;

    layer_wnd_ = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
        g_wnd_class,
        g_wnd_name,
        WS_POPUP,
        info.rcClient.left, info.rcClient.top, width, height,
        parent_wnd, NULL, inst_, this);
    if (!layer_wnd_) {
        auto err = GetLastError();
        veLOGE("Can not Create Layer Wnd, error: {}", err);
        return false;
    }

    MARGINS mar = { -1 };
    HRESULT hr = DwmExtendFrameIntoClientArea(layer_wnd_, &mar);
    if (FAILED(hr)) {
        veLOGW("Can not Extend Frame Into Client, error: {}", hr);
    }

    if (!initD2D1()) {
        veLOGE("Error InitD2D1");
        return false;
    }
    BYTE alpha_percent = 255;   // 0-255
    if (!SetLayeredWindowAttributes(layer_wnd_, RGB(0, 0, 0), alpha_percent, LWA_ALPHA)) {
        veLOGE("error in SetLayeredWindowAttributes");
    }
    if (!ShowWindow(layer_wnd_, SW_NORMAL)) {
        veLOGW("error in ShowWindow");
    }
    if (!UpdateWindow(layer_wnd_)) {
        veLOGW("error in update layer window");
        return false;
    }

    should_stop_ = false;
    return true;
}

void StatusLayerWnd::onPoseSizeChanged() {
    CHECK_LAYER_STATE;
    if (IsIconic(parent_wnd_)) {
        veLOGI("player window is iconic");
        return;
    }
    WINDOWINFO info = { sizeof(WINDOWINFO) };
    if (!GetWindowInfo(parent_wnd_, &info)) {
        veLOGW("GetWindowInfo failed, error: {}", GetLastError());
        return;
    }
    int width = info.rcClient.right - info.rcClient.left;
    int height = info.rcClient.bottom - info.rcClient.top;

    BOOL ret = SetWindowPos(layer_wnd_, NULL,
        info.rcClient.left, info.rcClient.top, width, height,
        SWP_SHOWWINDOW | SWP_NOACTIVATE);
    if (!ret) {
        veLOGW("set wnd pos failed");
    }

    if (!d2d_reader_target_) {
        veLOGE("reader target is null");
        return;
    }
    d2d_reader_target_->Resize(D2D1::SizeU(width, height));
    onRender();
}

void StatusLayerWnd::onParentActive(bool is_active) {
    WINDOWINFO info = { sizeof(WINDOWINFO) };
    if (!GetWindowInfo(parent_wnd_, &info)) {
        veLOGW("GetWindowInfo failed, error: {}", GetLastError());
        return;
    }
    int width = info.rcClient.right - info.rcClient.left;
    int height = info.rcClient.bottom - info.rcClient.top;

    BOOL ret = SetWindowPos(layer_wnd_, is_active ? HWND_TOPMOST : HWND_NOTOPMOST,
        info.rcClient.left, info.rcClient.top, width, height,
        SWP_NOACTIVATE);
    if (!ret) {
        veLOGW("set wnd pos failed, error: {}", GetLastError());
    }
}

bool StatusLayerWnd::onRender() {
    if (should_stop_) {
        return true;
    }

    if (!d2d_reader_target_) {
        veLOGW("render target is null");
        return false;
    }
    if (!has_frame_) {
        return false;
    }

    std::lock_guard<std::mutex> lock_g(render_mutex_);
    RECT client_rect;
    GetClientRect(parent_wnd_, &client_rect);
    int clinet_width = client_rect.right - client_rect.left;
    int client_height = client_rect.bottom - client_rect.top;

    d2d_reader_target_->BeginDraw();
    d2d_reader_target_->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));
    // draw string
    video_stats_ = fmt::format("stats: loss={:.1f}%, delay={}ms, rtt={}ms, frame_rate={}fps, res={}x{}, recv_kbps={}",
        static_cast<float>(loss_) * 100, delay_, rtt_, frame_rate_, frame_width_, frame_height_, recv_kbps_);
    drawString(video_stats_, static_cast<float>(clinet_width / font_size_rate_), static_cast<float>(clinet_width / font_size_rate_ / 2),
        static_cast<float>(clinet_width / font_size_rate_ / 4), D2D1::ColorF(D2D1::ColorF::SpringGreen, 1.0f));

    HRESULT hr = d2d_reader_target_->EndDraw();
    return hr == S_OK;
}

void StatusLayerWnd::setVideoStats(float loss, long delay, int rtt, int frame_rate, int recv_kbps) {
    loss_ = (isnan(loss) ? 0.0f : loss);
    rtt_ = rtt;
    frame_rate_ = frame_rate;
    recv_kbps_ = recv_kbps;
}

void StatusLayerWnd::setDetectDelay(int delay) {
    delay_ = delay;
}

void StatusLayerWnd::setFrameWH(int width, int height) {
    frame_width_ = width;
    frame_height_ = height;
}

void StatusLayerWnd::setHasFrame(bool has_frame) {
    has_frame_ = has_frame;
    if (!has_frame_ && d2d_reader_target_) {
        veLOGI("clear layer screean");
        d2d_reader_target_->BeginDraw();
        d2d_reader_target_->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));
        HRESULT hr = d2d_reader_target_->EndDraw();
    }
}

HWND StatusLayerWnd::getLayerWnd() {
    return layer_wnd_;
}

void StatusLayerWnd::drawString(std::string str, float font_size, float x, float y, const  D2D1::ColorF& color) {
    RECT re;
    GetClientRect(layer_wnd_, &re);
    FLOAT dpix, dpiy;
    dpix = static_cast<float>(re.right - re.left);
    dpiy = static_cast<float>(re.bottom - re.top);

    std::wstring wstr = stringToWstring(str);
    winrt::com_ptr<IDWriteTextLayout> dwrite_layout = nullptr;
    HRESULT res = dwrite_factory_->CreateTextLayout(
        wstr.c_str(), static_cast<UINT32>(wstr.length()),
        dwrite_format_.get(), dpix, dpiy, dwrite_layout.put());
    if (FAILED(res)) {
        veLOGW("create text layout failed");
        return;
    }

    DWRITE_TEXT_RANGE range = { 0, static_cast<UINT32>(wstr.length()) };
    dwrite_layout->SetFontSize(font_size, range);
    d2d_solid_brush_->SetColor(color);
    if (d2d_reader_target_ == nullptr) {
        veLOGW("reader target is null");
        return;
    }
    if (dwrite_layout == nullptr) {
        veLOGW("WriteLayout is null");
        return;
    }
    if (d2d_solid_brush_ == nullptr) {
        veLOGW("SolidBrush is null");
        return;
    }

    d2d_reader_target_->DrawTextLayout(D2D1::Point2F(x, y), dwrite_layout.get(), d2d_solid_brush_.get());
}

LRESULT StatusLayerWnd::wndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    LRESULT result = S_OK;
    switch (msg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default: {
            return DefWindowProc(wnd, msg, wparam, lparam);
        }
    }
    return result;
}

LRESULT CALLBACK readyLayerWndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    LRESULT ret = S_OK;
    if (msg == WM_NCCREATE) {
        LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT> (lparam);
        SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pcs->lpCreateParams));
        ret = DefWindowProc(wnd, msg, wparam, lparam);
    } else {
        auto* thiz = reinterpret_cast<StatusLayerWnd*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
        if (thiz) {
            ret = thiz->wndProc(wnd, msg, wparam, lparam);
        } else {
            return DefWindowProc(wnd, msg, wparam, lparam);
        }
    }
    return ret;
}

bool StatusLayerWnd::initD2D1() {
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2d_factory_.put());
    if (FAILED(hr)) {
        veLOGE("Can not Create d2d fatory, error: {}", hr);
        return false;
    }

    // Obtain the size of the drawing area.
    RECT rc;
    if (!GetClientRect(layer_wnd_, &rc)) {
        veLOGE("get client rect failed");
        return false;
    }
    hr = d2d_factory_->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            96.f, 96.f),
        D2D1::HwndRenderTargetProperties(layer_wnd_, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
        d2d_reader_target_.put());
    if (FAILED(hr)) {
        veLOGE("Can not Create d2d RenderTarget, error: {}", hr);
        return false;
    }

    hr = d2d_reader_target_->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), d2d_solid_brush_.put());
    if (FAILED(hr)) {
        veLOGE("Can not Create d2d SolidColorBrush, error: {}", hr);
        return false;
    }
    d2d_reader_target_->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)dwrite_factory_.put());
    if (FAILED(hr)) {
        veLOGE("Can not Call DWriteCreateFactory, error: {}", hr);
        return false;
    }
    hr = dwrite_factory_->CreateTextFormat(font_name_.c_str(), NULL, DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10.0f, L"en-us", dwrite_format_.put());
    if (FAILED(hr)) {
        veLOGE("Can notCreateTextFormat, error: {}", hr);
        return false;
    }

    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wic_factory_));
    if (FAILED(hr)) {
        veLOGE("Can not Create Wic Factory, error: {}", hr);
        return false;
    }

    return true;
}

void StatusLayerWnd::cleanUp() {
    if (!should_stop_) {
        should_stop_ = true;
        wic_factory_ = nullptr;
    }
}

HRESULT StatusLayerWnd::loadD2DBitmapFromFile(const std::string& img_file, int width, int height, winrt::com_ptr<ID2D1Bitmap>& d2d_bitmap) {
    if (!wic_factory_) {
        veLOGE("create bitmap from file error");
        return E_FAIL;
    }

    HRESULT hr = S_OK;
    std::wstring file_name = stringToWstring(img_file);
    winrt::com_ptr<IWICBitmapDecoder> wic_decoder = nullptr;
    winrt::com_ptr<IWICBitmapFrameDecode> wic_source = nullptr;
    winrt::com_ptr<IWICFormatConverter> wic_converter = nullptr;
    // Decode the source image
    hr = wic_factory_->CreateDecoderFromFilename(
        file_name.c_str(),                          // Image to be decoded
        NULL,                                       // Do not prefer a particular vendor
        GENERIC_READ,                               // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,             // Cache metadata when needed
        wic_decoder.put()       // Pointer to the decoder
    );
    if (FAILED(hr)) {
        veLOGW("CreateDecoderFromFilename failed, error: {}", hr);
        return hr;
    }

    // Retrieve the first frame of the image from the decoder
    hr = wic_decoder->GetFrame(0, wic_source.put());
    if (FAILED(hr)) {
        veLOGW("Decoder GetFrame failed, error: {}", hr);
        return hr;
    }

    winrt::com_ptr<IWICBitmapScaler> wic_scaler = nullptr;
    hr = wic_factory_->CreateBitmapScaler(wic_scaler.put());
    if (FAILED(hr)) {
        veLOGW("CreateBitmapScaler failed, error: {}", hr);
        return hr;
    }
    hr = wic_scaler->Initialize(wic_source.get(), width, height, WICBitmapInterpolationModeCubic);
    if (FAILED(hr)) {
        veLOGW("IWICBitmapScaler Initialize failed, error: {}", hr);
        return hr;
    }

    // Format convert the frame to 32bppPBGRA
    hr = wic_factory_->CreateFormatConverter(wic_converter.put());
    if (FAILED(hr)) {
        veLOGW("IWICFactory CreateFormatConverter failed, error: {}", hr);
        return hr;
    }

    hr = wic_converter->Initialize(
        wic_scaler.get(),                // Input bitmap to convert
        GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
        WICBitmapDitherTypeNone,         // Specified dither pattern
        NULL,                            // Specify a particular palette 
        0.f,                             // Alpha threshold
        WICBitmapPaletteTypeCustom       // Palette translation type
    );
    if (FAILED(hr)) {
        veLOGW("ConvertedSourceBitmap Initialize failed, error: {}", hr);
        return hr;
    }

    // Need to release the previous D2DBitmap if there is one
    hr = d2d_reader_target_->CreateBitmapFromWicBitmap(wic_converter.get(), NULL, d2d_bitmap.put());
    if (FAILED(hr)) {
        veLOGW("CreateBitmapFromWicBitmap failed, error: {}", hr);
        return hr;
    }
    return hr;
}

