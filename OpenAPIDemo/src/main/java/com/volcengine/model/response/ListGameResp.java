package com.volcengine.model.response;

import com.alibaba.fastjson.annotation.JSONField;
import lombok.Data;
import java.util.List;

@Data
public class ListGameResp {
    @JSONField(name = "ResponseMetadata")
    ResponseMetadata responseMetadata;
    @JSONField(name = "Result")
    ResultBean result;

    @Data
    public static class ResultBean {
        @JSONField(name = "row")
        List<Game> list;
        @JSONField(name = "total")
        int total;
    }

    @Data
    public static class Game {
        @JSONField(name = "game_id")
        private String gameId;
    }
}
