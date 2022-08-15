/**
 * Copyright (c) 2022 Volcengine
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package com.volcengine.vegameengine

import android.Manifest
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import androidx.appcompat.app.AppCompatActivity
import com.blankj.utilcode.util.PermissionUtils

class MainActivity : AppCompatActivity() {

    private lateinit var mBtnStartGame: Button

    // 云游戏
    private lateinit var mGameIdEditText: EditText
    private lateinit var mRoundEditText: EditText
    private lateinit var mClarityEditText: EditText

    private val testBean =
        TestBean(gameId = "7112353257888193324", roundId = "123")


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        initView()
    }

    private fun initView() {
        mBtnStartGame = findViewById(R.id.btn_start_game)
        mGameIdEditText = findViewById(R.id.editText_gameId)
        mRoundEditText = findViewById(R.id.editText_roundId)
        mClarityEditText = findViewById(R.id.editText_clarityId)

        mBtnStartGame.setOnClickListener {

            GameActivity.startGame(
                mGameIdEditText.text.toString(),
                mRoundEditText.text.toString(),
                mClarityEditText.text.toString().toIntOrNull()?:1,
                this@MainActivity
            )
        }
        mGameIdEditText.setText(testBean.gameId)
        mRoundEditText.setText(testBean.roundId)
        mClarityEditText.setText(testBean.clarityId)

        PermissionUtils.permission(
            Manifest.permission.CAMERA,
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.BLUETOOTH,
        ).request()
    }


    data class TestBean(
        var gameId: String,
        var roundId: String,
        var clarityId: String? = "1",
        var engineType: String = "BYTE_RTC"
    )
}