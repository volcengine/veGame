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
import android.content.SharedPreferences
import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import android.widget.Button
import android.widget.EditText
import androidx.appcompat.app.AppCompatActivity
import com.blankj.utilcode.util.PermissionUtils

class MainActivity : AppCompatActivity() {

    private lateinit var mBtnStartGame: Button

    // 云游戏
    private lateinit var mGameIdEditText: EditText
    private lateinit var mClarityEditText: EditText
    private lateinit var mRoundIdEditText: EditText

    private var mGameId: String = "7104356860098059039"
    private var mRoundId: String = "123"
    private var mClarityId = "1"
    private lateinit var mSp: SharedPreferences

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mSp = getSharedPreferences("demo_sp", MODE_PRIVATE)
        mGameId = mSp.getString("game_id", mGameId).toString()
        mRoundId = mSp.getString("round_id", mRoundId).toString()
        setContentView(R.layout.activity_main)
        initView()
    }

    private fun initView() {
        mBtnStartGame = findViewById(R.id.btn_start_game)
        mGameIdEditText = findViewById(R.id.editText_gameId)
        mClarityEditText = findViewById(R.id.editText_clarityId)
        mRoundIdEditText = findViewById(R.id.editText_roundId)

        mBtnStartGame.apply {
            setOnClickListener {
                GameActivity.startGame(
                    mGameId,
                    mRoundId,
                    mClarityId.toInt(),
                    this@MainActivity
                )
            }
        }
        mGameIdEditText.setText(mGameId)
        mGameIdEditText.apply {
            addTextChangedListener(object : TextWatcher {
                override fun afterTextChanged(s: Editable) {}

                override fun beforeTextChanged(
                    s: CharSequence, start: Int,
                    count: Int, after: Int
                ) {
                }

                override fun onTextChanged(
                    s: CharSequence, start: Int,
                    before: Int, count: Int
                ) {
                    mGameId = s.toString()
                    mSp.edit().putString("game_id", mGameId).apply()
                }
            })
        }

        mRoundIdEditText.setText(mRoundId)
        mRoundIdEditText.apply {
            addTextChangedListener(object : TextWatcher {
                override fun afterTextChanged(s: Editable) {}

                override fun beforeTextChanged(
                    s: CharSequence, start: Int,
                    count: Int, after: Int
                ) {
                }

                override fun onTextChanged(
                    s: CharSequence, start: Int,
                    before: Int, count: Int
                ) {
                    mRoundId = s.toString()
                    mSp.edit().putString("round_id", mRoundId).apply()
                }
            })
        }

        mClarityEditText.addTextChangedListener(object : TextWatcher {
            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {
            }

            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                mClarityId = s.toString()
            }

            override fun afterTextChanged(s: Editable?) {

            }
        })

        if (!PermissionUtils.isGranted(
                Manifest.permission.CAMERA,
                Manifest.permission.RECORD_AUDIO
            )
        ) {
            PermissionUtils.permission(
                Manifest.permission.CAMERA, Manifest.permission.RECORD_AUDIO
            ).request()
        }

    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
    }
}