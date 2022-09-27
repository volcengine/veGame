package com.volcengine.vegameengine

import android.annotation.SuppressLint
import android.content.Intent
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.webkit.WebView

class WebViewActivity : AppCompatActivity() {

    private lateinit var webView: WebView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_web_view)
        webView = findViewById(R.id.web_view)
        intent.getStringExtra("uri")?.let { webView.loadUrl(it) }
    }


//    companion object {
////        const val KEY_URL = "uri"
//        fun start(uri: String, activity: AppCompatActivity) {
//            val intent = Intent(activity, WebViewActivity::class.java)
//            intent.putExtra("uri", uri)
//            activity.startActivity(intent)
//        }
//    }
}