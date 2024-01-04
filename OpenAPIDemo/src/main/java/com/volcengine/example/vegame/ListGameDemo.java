package com.volcengine.example.vegame;

import com.alibaba.fastjson.JSON;
import com.volcengine.model.request.ListGameReq;
import com.volcengine.model.response.ListGameResp;
import com.volcengine.service.vegame.IVeGameService;
import com.volcengine.service.vegame.impl.VeGameServiceImpl;

public class ListGameDemo {
    public static void main(String[] args) {
        IVeGameService veGameService = VeGameServiceImpl.getInstance();

        String accessKey = System.getenv("VEGAME_ACCESS_KEY");
        String secrteKey = System.getenv("VEGAME_SECRET_KEY");


        veGameService.setAccessKey(accessKey);
        veGameService.setSecretKey(secrteKey);

        try {
            ListGameReq req = new ListGameReq();

            req.setOffset(0);
            req.setLimit(10);
            req.setProductId("1498846613274103808");

            ListGameResp resp = veGameService.ListGame(req);

            System.out.println(JSON.toJSONString(resp));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
