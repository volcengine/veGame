package com.volcengine.vegameengine

data class TestBean(
    var gameId: String,
    var roundId: String?= "123",
    var clarityId: String? = "1",
    var engineType: String = "BYTE_RTC",
    var reservedId: String? = ""
)
