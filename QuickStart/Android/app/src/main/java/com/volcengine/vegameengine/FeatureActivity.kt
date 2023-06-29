package com.volcengine.vegameengine

import android.Manifest
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import com.blankj.utilcode.util.PermissionUtils
import com.volcengine.vegameengine.base.BaseSampleActivity
import com.volcengine.vegameengine.util.AssetsUtil
import com.volcengine.vegameengine.util.Feature

class FeatureActivity : BaseSampleActivity() {

    private var mFeatureId = -1

    private lateinit var etGameId: EditText
    private lateinit var etRoundId: EditText
    private lateinit var etClarityId: EditText
    private lateinit var etReservedId: EditText
    private lateinit var btnStartGame: Button

    // 这里请输入你的gameId
    private val testBean = TestBean("your_game_id") // 消息通道

    override fun onCreate(savedInstanceState: Bundle?) {
        mFeatureId = intent.getIntExtra("featureId", -1)
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_startgame)
        initView()
    }

    private fun initView() {
        etGameId = findViewById(R.id.editText_gameId)
        etRoundId = findViewById(R.id.editText_roundId)
        etClarityId = findViewById(R.id.editText_clarityId)
        etReservedId = findViewById(R.id.editText_reservedId)
        etGameId.setText(testBean.gameId)
        etRoundId.setText(testBean.roundId)
        etClarityId.setText(testBean.clarityId)
        etReservedId.setText(testBean.reservedId)

        btnStartGame = findViewById(R.id.btn_start_game)
        btnStartGame.setOnClickListener{
            if (!AssetsUtil.isAssetsFileExists(applicationContext, "sts.json")) {
                Toast.makeText(this, "Assets目录下sts.json文件不存在，请先创建文件！", Toast.LENGTH_SHORT).show()
            }
            else if (etGameId.text.isEmpty()) {
                Toast.makeText(this, "请输入gameId", Toast.LENGTH_SHORT).show()
            }
            else {
                GameActivity.startGame(
                    etGameId.text.toString(),
                    etRoundId.text.toString(),
                    etClarityId.text.toString().toIntOrNull() ?: 1,
                    etReservedId.text.toString(),
                    this,
                    mFeatureId
                )
            }
        }

        PermissionUtils.permission(
            Manifest.permission.CAMERA,
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.BLUETOOTH,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.ACCESS_COARSE_LOCATION,
            Manifest.permission.ACCESS_FINE_LOCATION
        ).request()
    }

    override fun titleRes(): Int {
        when (mFeatureId) {
            Feature.FEATURE_AUDIO -> {
                return R.string.audio
            }
            Feature.FEATURE_CAMERA -> {
                return R.string.camera
            }
            Feature.FEATURE_CLIPBOARD -> {
                return R.string.clipboard
            }
            Feature.FEATURE_FILE_CHANNEL -> {
                return R.string.file_channel
            }
            Feature.FEATURE_FILE_CHANNEL_EXT -> {
                return R.string.file_channel_ext
            }
            Feature.FEATURE_LOCAL_INPUT -> {
                return R.string.local_input
            }
            Feature.FEATURE_LOCATION -> {
                return R.string.location
            }
            Feature.FEATURE_MESSAGE_CHANNEL -> {
                return R.string.message_channel
            }
            Feature.FEATURE_MULTI_USER -> {
                return R.string.multi_user
            }
            Feature.FEATURE_GAME_PAD -> {
                return R.string.game_pad
            }
            Feature.FEATURE_POD_CONTROL -> {
                return R.string.pod_control
            }
            Feature.FEATURE_PROBE_NETWORK -> {
                return R.string.probe_network
            }
            Feature.FEATURE_SENSOR -> {
                return R.string.sensor
            }
            Feature.FEATURE_UNCLASSIFIED -> {
                return R.string.unclassified
            }
            else -> {
                return -1
            }
        }
    }
}