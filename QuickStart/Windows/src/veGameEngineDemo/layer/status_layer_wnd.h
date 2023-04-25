#pragma once

class StatusLayerWnd {
public:
    StatusLayerWnd() = default;
    virtual ~StatusLayerWnd();

    // wnd
    bool open(HINSTANCE inst, HWND parent_wnd);
    LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    // event
    void onPoseSizeChanged();
    void onParentActive(bool is_active);
    bool onRender();

    // get / set
    void setVideoStats(float loss, long delay, int rtt, int frame_rate, int recv_kpbs);
    void setDetectDelay(int delay);
    void setFrameWH(int width, int height);
    void setHasFrame(bool has_frame);
    HWND getLayerWnd();

    // Draw Method
    void drawString(std::string str, float font_size, float x, float y, const D2D1::ColorF& color);

protected:
    bool registerWndClass();
    bool initD2D1();
    void cleanUp();

    HRESULT loadD2DBitmapFromFile(const std::string& img_file, int width, int height, winrt::com_ptr<ID2D1Bitmap>& d2d_bitmap);

private:
    std::mutex render_mutex_;
    std::atomic<bool> should_stop_ = true;
    // video stats
    bool has_frame_ = false;
    float loss_ = 0.0f;
    long delay_ = 0;
    int rtt_ = 0;
    int frame_rate_ = 0;
    int recv_kbps_ = 0;
    int frame_width_ = 0;
    int frame_height_ = 0;
    std::string video_stats_;
    int font_size_rate_ = 96;

    HINSTANCE inst_ = nullptr;
    HWND parent_wnd_ = nullptr;
    HWND layer_wnd_ = nullptr;
    // D2D
    std::wstring font_name_ = L"arial";
    winrt::com_ptr<ID2D1Factory> d2d_factory_ = nullptr;
    winrt::com_ptr<ID2D1HwndRenderTarget> d2d_reader_target_ = nullptr;
    winrt::com_ptr<ID2D1SolidColorBrush> d2d_solid_brush_ = nullptr;
    winrt::com_ptr<IDWriteFactory> dwrite_factory_ = nullptr;
    winrt::com_ptr<IDWriteTextFormat> dwrite_format_ = nullptr;
    IWICImagingFactory* wic_factory_ = nullptr;
};

