package com.iamjinge.freetype;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Path;
import android.graphics.RectF;
import android.graphics.Typeface;
import android.support.annotation.Nullable;
import android.text.Layout.Alignment;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import com.iamjinge.freetype.PathExtractor.Metrics;

public class CustomView extends View {
    private static final String TAG = "CustomView";
    private Paint paint = new Paint();
    private TextPaint textPaint = new TextPaint();
    private Path path = new Path();
    private RectF oBoxF = new RectF();
    private RectF boxF = new RectF();
    private int bLine;
    private PathExtractor pathExtractor;
    private Metrics m = new Metrics();

    public CustomView(@Nullable Context context) {
        this(context, null);
    }

    public CustomView(@Nullable Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public CustomView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.pathExtractor = new PathExtractor("/sdcard/temp/font.ttf");

        this.paint.setStyle(Style.STROKE);
        this.paint.setStrokeWidth(2.0F);
        this.paint.setColor(Color.BLUE);

        this.textPaint.setAntiAlias(true);
        this.textPaint.setStyle(Style.FILL);
        this.textPaint.setColor(Color.RED);
        this.textPaint.setTextSize(100.0F);
        this.textPaint.setTypeface(Typeface.createFromAsset(context.getAssets(), "font.ttf"));
        this.pathExtractor.setTextSize(100);
        this.pathExtractor.getMetrics(this.m);

        Log.d(TAG, "height " + this.m.height + ", " + this.m.ascender + ", " + this.m.descender);
        boolean success = this.pathExtractor.extractPath('A', this.path, this.boxF);

        StaticLayout layout = new StaticLayout("A", this.textPaint, 500, Alignment.ALIGN_NORMAL, 1.0F, 0.0F, false);
        this.oBoxF.set(layout.getLineLeft(0), layout.getLineTop(0), layout.getLineRight(0), layout.getLineBottom(0));
        this.paint.getFontMetrics();
        Log.d(TAG, "" + layout.getLineAscent(0) + " and " + layout.getLineDescent(0) + " and " + layout.getLineBaseline(0));
        this.bLine = layout.getLineBaseline(0);
    }


    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        float x = 100.0F;
        float y = 200.0F;

        canvas.drawLine(0, y, 1000, y, this.paint);
        canvas.drawLine(x, 0.0F, x, 1000.0F, this.paint);

        canvas.save();
        canvas.translate(x, y);
        canvas.drawText("A", 0.0F, (float) this.bLine + 0.0F, this.textPaint);
        canvas.drawRect(this.oBoxF, this.paint);
        canvas.restore();

        x = 300.0F;
        canvas.drawLine(0.0F, y, 1000.0F, y, this.paint);
        canvas.drawLine(x, 0.0F, x, 1000.0F, this.paint);
        canvas.save();
        canvas.translate(x, y + this.m.ascender);
        canvas.drawPath(this.path, (Paint) this.textPaint);
        canvas.drawRect(this.boxF, this.paint);
        canvas.restore();
    }

}