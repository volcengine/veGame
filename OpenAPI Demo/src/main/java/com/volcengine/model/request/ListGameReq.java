package com.volcengine.model.request;


import com.alibaba.fastjson.annotation.JSONField;
import lombok.Data;

@Data
public class ListGameReq {
    @JSONField(name = "limit")
    int limit;
    @JSONField(name = "offset")
    int offset;
    @JSONField(name = "product_id")
    String productId;

    public int getLimit() {
        if (this.limit == 0) {
            return 10;
        } else {
            return limit;
        }
    }
}
