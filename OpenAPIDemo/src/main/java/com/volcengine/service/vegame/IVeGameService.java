package com.volcengine.service.vegame;

import com.volcengine.service.IBaseService;
import com.volcengine.model.request.ListGameReq;
import com.volcengine.model.response.ListGameResp;

public interface IVeGameService extends IBaseService {
    ListGameResp ListGame(ListGameReq req) throws Exception;
}
