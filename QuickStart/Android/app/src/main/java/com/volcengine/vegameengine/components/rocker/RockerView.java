package com.volcengine.vegameengine.components.rocker;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PointF;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewTreeObserver;

import java.io.IOException;
import java.io.InputStream;

/**
 * 通过方向盘滑动的方式，模拟W、A、S、D 4个键盘按键及组合键
 *
 * @author bytedance
 */
public class RockerView extends View {

    private static final String RES_DIR_NAME = "rocker";

    private static final String TAG = "RockerView";

    /**
     * 摇杆半径和盘子半径的比例
     */
    private float mProportionOfRocker;

    private PointF mCenterPoint;

    /**
     * 整个盘子的半径
     */
    private float mDiskRadius;

    /**
     * 顺时针，和十二点方向夹角
     */
    private float mAngle;

    // 盘子背景图
    private Bitmap mNormalDisk;

    // 摇杆背景图
    private Bitmap mRocker;

    private Paint mPaint;

    private boolean isControl;
    private PointF mRockerPoint;

    private OnRockerChangeListener mListener;

    private OnRockerLocationListener locationListener;

    private static final int TRACE_NO_POINTER = -1;
    private int mPointerIdTrace = TRACE_NO_POINTER;

    public RockerView(Context context) {
        super(context, null);
        initVariable();
        loadRes();
    }

    public RockerView(Context context, AttributeSet attributeSet){
        super(context, attributeSet);
        initVariable();
        loadRes();
    }

    private void initVariable() {
        mAngle = 0;
        mRockerPoint = new PointF();
        mPaint = new Paint();
        //用Canvas绘制Bitmap会有锯齿，通过给Paint设置抗锯齿
        mPaint.setAntiAlias(true);
        mPaint.setFilterBitmap(true);
    }

    private void loadRes() {
        clearOldRes();
        mNormalDisk = loadBitmapFromAssets(getContext(), Constant.ROCKER_NORMAL_L + ".png");
        mRocker = loadBitmapFromAssets(getContext(),Constant.ROCKER_CENTER_L + ".png");
        if (mRocker != null) {
            mProportionOfRocker = mRocker.getWidth() * 1f / mNormalDisk.getWidth();
            // 计算圆心坐标及半径
            getViewTreeObserver().addOnPreDrawListener(new ViewTreeObserver.OnPreDrawListener() {
                @Override
                public boolean onPreDraw() {
                    getViewTreeObserver().removeOnPreDrawListener(this);
                    mCenterPoint = new PointF(getWidth() / 2f, getHeight() / 2f);
                    mDiskRadius = getWidth() / 2f;
                    return false;
                }
            });
        }
    }

    private void clearOldRes(){
        if (mNormalDisk != null) {
            mNormalDisk.recycle();
        }
        if (mRocker != null) {
            mRocker.recycle();
        }
    }

    private static Bitmap loadBitmapFromAssets(Context context, String resName) {
        InputStream input = null;
        Bitmap result = null;
        try {
            input = context.getAssets().open(RES_DIR_NAME+ "/" + resName);
            result = BitmapFactory.decodeStream(input);
        } catch (IOException e) {
            Log.e(TAG, resName + "load fail.", e);
        }finally {
            if (input != null){
                try {
                    input.close();
                } catch (IOException e) {
                   // ignore exception
                }
            }
        }
        return result;
    }

    private final Rect mDrawRect = new Rect();

    private Rect getDiskRect() {
        mDrawRect.set(0, 0, getWidth(), getHeight());
        return mDrawRect;
    }

    private Rect getRockerRect(float centralX, float centralY) {
        mDrawRect.left = (int) (centralX - mDiskRadius * mProportionOfRocker);
        mDrawRect.top = (int) (centralY - mDiskRadius * mProportionOfRocker);
        mDrawRect.right = (int) (centralX + mDiskRadius * mProportionOfRocker);
        mDrawRect.bottom = (int) (centralY + mDiskRadius * mProportionOfRocker);
        return mDrawRect;
    }

    private int calculateTouchInfo(float x, float y) {
        updateRockerAndAngle(x, y);
        return takeDirectionInEightKey();
    }

    private void updateRockerAndAngle(float x, float y) {
        float relativeX = x - mCenterPoint.x;
        float relativeY = y - mCenterPoint.y;
        float distance = (float) Math.sqrt(Math.pow(relativeX, 2) + Math.pow(relativeY, 2));
        // 手指必须移动多少距离才会触发rock回调，
        // 默认是中间圆形摇杆的半径，
        // 业务侧可根据自身实际情况，scale局部变量limit，来调整此组件的灵敏度
        float limit = mDiskRadius * mProportionOfRocker;
        float aCos = (float) (Math.acos(relativeY / distance) * 180 / Math.PI);
        if (distance <= limit) {
            mRockerPoint.set(x, y);
            mAngle = -1;
        } else {
            if(distance <= mDiskRadius){
                mRockerPoint.set(x, y);
            } else {
                //触摸区域超出范围，绘制成Rocker和Disk相切
                float scale = mDiskRadius * (1.0f - mProportionOfRocker) / distance;
                float newAnchorX = relativeX * scale + mCenterPoint.x;
                float newAnchorY = relativeY * scale + mCenterPoint.y;
                mRockerPoint.set(newAnchorX, newAnchorY);
            }
            //根据View的左、右半边进行计算
            mAngle = relativeX > 0 ? 180 - aCos : 180 + aCos;
        }
    }

    private void handleRockerChange(int action, int direction) {
        if (mListener != null) {
            mListener.onRock(action, direction);
        }
    }

    private int takeDirectionInEightKey() {
        int rad = Constant.DIRECTION_NONE;
        if((mAngle > 330 && mAngle <= 360)
            || (mAngle >= 0 && mAngle <= 30)){
            // 330度到30度，为正上方
            rad = Constant.DIRECTION_UP;
        } else if (mAngle > 30 && mAngle <= 60){
            // 30度到60度，为右上方
            rad = Constant.DIRECTION_RIGHT_UP;
        } else if (mAngle > 60 && mAngle <= 120){
            // 60度到120度，为正右方
            rad = Constant.DIRECTION_RIGHT;
        } else if (mAngle > 120 && mAngle <= 150){
            // 120度到150度，为右下方
            rad = Constant.DIRECTION_RIGHT_DOWN;
        } else if(mAngle > 150 && mAngle <= 210){
            // 150度到210度，为正下方
            rad = Constant.DIRECTION_DOWN;
        } else if(mAngle > 210 && mAngle <= 240){
            // 210度到240度，为左下方
            rad = Constant.DIRECTION_LEFT_DOWN;
        } else if(mAngle > 240 && mAngle <= 300){
            // 240度到300度，为正左方
            rad = Constant.DIRECTION_LEFT;
        } else if(mAngle > 300 && mAngle <= 330){
            // 300度到330度，为左上方
            rad = Constant.DIRECTION_LEFT_UP;
        }
        return rad;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getActionMasked()) {
            case MotionEvent.ACTION_POINTER_DOWN:
            case MotionEvent.ACTION_DOWN:
                if (mPointerIdTrace == TRACE_NO_POINTER && isTouchMe(event)) {
                    mPointerIdTrace = event.getPointerId(event.getActionIndex());
                    if(locationListener != null) {
                        locationListener.onRockLocationUpdated(event.getX(), event.getY(), mDiskRadius);
                    }
                }
            case MotionEvent.ACTION_MOVE: {
                if (event.getPointerId(event.getActionIndex()) == mPointerIdTrace) {
                    int direction = calculateTouchInfo(event.getX(), event.getY());
                    isControl = true;
                    handleRockerChange(MotionEvent.ACTION_DOWN, direction);
                    if(locationListener != null) {
                        locationListener.onRockLocationUpdated(event.getX(), event.getY(), mDiskRadius);
                    }
                    break;
                }
            }
            case MotionEvent.ACTION_CANCEL:
            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_UP: {
                if (event.getPointerId(event.getActionIndex()) == mPointerIdTrace) {
                    int direction = calculateTouchInfo(event.getX(), event.getY());
                    isControl = false;
                    mAngle = 0;
                    handleRockerChange(MotionEvent.ACTION_UP, direction);
                    mPointerIdTrace = TRACE_NO_POINTER;
                    if(locationListener != null) {
                        locationListener.onRockLocationUpdated(event.getX(), event.getY(), mDiskRadius);
                    }
                }
                break;
            }
            default:
        }
        postInvalidate();
        return true;
    }

    private boolean isTouchMe(MotionEvent event){
        float x = event.getX(event.getActionIndex());
        float y = event.getY(event.getActionIndex());
        return !(x < 0 || y < 0 || x > getMeasuredWidth() || y > getMeasuredHeight());
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        int width = MeasureSpec.getSize(widthMeasureSpec);
        int height = MeasureSpec.getSize(heightMeasureSpec);
        int size = Math.min(width, height);
        setMeasuredDimension(size, size);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        canvas.drawBitmap(mNormalDisk, null, getDiskRect(), mPaint);
        if (isControl) {
            Rect rockerRect = getRockerRect(mRockerPoint.x, mRockerPoint.y);
            canvas.drawBitmap(mRocker, null, rockerRect, mPaint);
        } else {
            if (mCenterPoint != null) {
                Bitmap bitmap = mRocker;
                canvas.drawBitmap(bitmap, null, getRockerRect(mCenterPoint.x, mCenterPoint.y), mPaint);
            }
        }
    }

    public void setOnRockerChangeListener(OnRockerChangeListener listener) {
        this.mListener = listener;
    }

    public void setOnRockerLocationListener(OnRockerLocationListener listener){
        this.locationListener = listener;
    }

    public interface OnRockerChangeListener {
        /**
         *
         * @param action     0表示摇杆按住，1表示摇杆松开
         * @param direction  表示摇杆所在方向
         */
        void onRock(int action, int direction);
    }

    /**
     * 摇杆中心相对位置变化监听器
     */
    public interface OnRockerLocationListener {
        /**
         *
         * @param x          当前触摸事件的X坐标
         * @param y          当前触摸事件的Y坐标
         * @param diskRadius 底盘半径
         */
        void onRockLocationUpdated(float x, float y, float diskRadius);
    }
}
