package com.volcengine.vegameengine.util

import androidx.preference.PreferenceManager
import com.volcengine.vegameengine.InitApplication


fun putPrefString(key: String, value: String) {
    PreferenceManager.getDefaultSharedPreferences(
        InitApplication.getInstance()
    ).edit().putString(key, value).apply()
}

fun getPrefString(key: String, defaultValue: String): String {
    return PreferenceManager.getDefaultSharedPreferences(
        InitApplication.getInstance()
    ).getString(key, defaultValue)!!
}

fun putPrefBoolean(key: String, boolean: Boolean) {
    return PreferenceManager.getDefaultSharedPreferences(
        InitApplication.getInstance()
    ).edit().putBoolean(key, boolean).apply()
}

fun getPrefBoolean(key: String, defaultValue: Boolean): Boolean {
    return PreferenceManager.getDefaultSharedPreferences(
        InitApplication.getInstance()
    ).getBoolean(key, defaultValue)
}

fun putPrefInt(key: String, value: Int) {
    PreferenceManager.getDefaultSharedPreferences(
        InitApplication.getInstance()
    ).edit().putInt(key, value).apply()
}

fun getPrefInt(key: String, defaultValue: Int): Int {
    return PreferenceManager.getDefaultSharedPreferences(
        InitApplication.getInstance()
    ).getInt(key, defaultValue)
}