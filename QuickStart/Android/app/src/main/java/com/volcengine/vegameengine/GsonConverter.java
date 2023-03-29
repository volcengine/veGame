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

package com.volcengine.vegameengine;

import com.google.gson.Gson;
import com.google.gson.internal.Primitives;
import com.google.gson.stream.JsonReader;
import com.volcengine.common.innerapi.IJsonConverter;
//import com.volcengine.androidcloud.common.api.IJsonConverter;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Type;

public class GsonConverter implements IJsonConverter {
    Gson gson;


    public GsonConverter() {
        gson = new Gson();
    }

    @Override
    public <T> T fromJson(String json, Class<T> cls) {
        if (json == null) {
            return null;
        }
        try {
            return gson.fromJson(json, cls);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

//    @Override
//    public <T> T fromJson(InputStream json, Class<T> cls) {
//        try {
//            JsonReader reader = new JsonReader(new InputStreamReader(json));
//            return Primitives.wrap(cls).cast(gson.fromJson(reader, cls));
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//        return null;
//    }

    @Override
    public <T> String toJson(T object) {
        if (object == null) {
            return null;
        }
        return gson.toJson(object);
    }

    @Override
    public <T> T fromJson(String json, Type type) {
        try {
            return gson.fromJson(json, type);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
