<?php

// 设置时区为协调世界时间(UTC)
date_default_timezone_set('UTC');

// 以下参数视服务不同而不同，一个服务内通常是一致的，以下AK SK 需要从火山密钥管理处获得
$Service = "veGame";
$Version = "2022-02-10";
$Region = "cn-north-1";
$Host = "open.volcengineapi.com";
$ContentType = "application/x-www-form-urlencoded";
$AK = "";
$SK = "";

// 函数：规范化查询字符串
function norm_query($params) {
    $query = "";
    ksort($params);
    foreach ($params as $key => $value) {
        if (is_array($value)) {
            foreach ($value as $v) {
                $query .= urlencode($key) . "=" . urlencode($v) . "&";
            }
        } else {
            $query .= urlencode($key) . "=" . urlencode($value) . "&";
        }
    }
    return rtrim($query, "&");
}

// sha256 非对称加密
function hmac_sha256($key, $content) {
    return hash_hmac("sha256", $content, $key);
}

// sha256 hash算法
function hash_sha256($content) {
    return hash("sha256", $content);
}

// 函数：发送请求并处理签名
function request($method, $date, $query, $header, $ak, $sk, $action, $body) {
    $credential = array(
        "access_key" => $ak,
        "secret_key" => $sk,
        "service" => "veGame",
        "region" => "cn-north-1"
    );

    $request_param = array(
        "body" => $body,
        "host" => "open.volcengineapi.com",
        "path" => "/",
        "method" => $method,
        "content_type" => "application/x-www-form-urlencoded",
        "date" => $date,
        "query" => array(
            "Action" => $action,
            "Version" => "2022-02-10",
            "ak" => $ak,
            "sk" => $sk,
            "expire" => "300"
        )
    );

    if ($body === null) {
        $request_param["body"] = "{}";
    }

    $x_date = strftime("%Y%m%dT%H%M%SZ", $request_param["date"]);
    $short_x_date = substr($x_date, 0, 8);
    $x_content_sha256 = hash_sha256($request_param["body"]);
    //var_dump($x_content_sha256);

    $sign_result = array(
        "Host" => $request_param["host"],
        "X-Date" => $x_date,
        "X-Content-Sha256" => $x_content_sha256,
        "Content-Type" => $request_param["content_type"]
    );

    $signed_headers_str = "host;x-date;x-content-sha256;content-type";
    $canonical_request_str = strtoupper($request_param["method"]) . "\n" . $request_param["path"] . "\n" . norm_query($request_param["query"]) . "\n" . 
	    "host:" . $request_param["host"] . "\n" .
	    "x-date:" . $x_date . "\n" . 
	    "x-content-sha256:" . $x_content_sha256 . "\n" . 
	    "content-type:" . $request_param["content_type"] . "\n\n" .
	    $signed_headers_str . "\n" . $x_content_sha256;
	    
    $hashed_canonical_request = hash_sha256($canonical_request_str);
    $credential_scope = $short_x_date . "/" . $credential["region"] . "/" . $credential["service"] . "/request";
    $string_to_sign = "HMAC-SHA256\n" . $x_date . "\n" . $credential_scope . "\n" . $hashed_canonical_request;
    //echo "string_to_sign: \n" ;
    //var_dump($string_to_sign);
    $k_date = hmac_sha256($sk, $short_x_date);
    //echo ">>>> sk=" . $sk . ", date=" . $short_x_date . PHP_EOL;
    //var_dump($k_date);
    $k_region = hmac_sha256(hex2bin($k_date), $credential["region"]);
    //echo ">>>> kDate=" . $k_date . ", region=" . $credential["region"] . PHP_EOL;
    //var_dump($k_region);
    $k_service = hmac_sha256(hex2bin($k_region), $credential["service"]);
    //echo ">>>> kRegion=" . $k_region . ", service=" . $credential["service"] . PHP_EOL;
    //var_dump($k_service);
    $k_signing = hmac_sha256(hex2bin($k_service), "request");
    //echo ">>>> kService=" . $k_service. ", request" . PHP_EOL;
    //var_dump($k_signing);
    $signature = hash_hmac("sha256", $string_to_sign, hex2bin($k_signing));

    $sign_result["Authorization"] = "HMAC-SHA256 Credential=" . $credential["access_key"] . "/" . $credential_scope . ", SignedHeaders=" . $signed_headers_str . ", Signature=" . $signature;
    
    $header = array_merge($header, $sign_result);
    $ch_headers = array();
    foreach ($header as $k => $v) {
	    $ch_hdr = $k . ": " . $v;
	    $ch_headers[]= $ch_hdr;
    }
    //var_dump($ch_headers);

    $url = "https://" . $request_param["host"] . $request_param["path"];
    $params = $request_param["query"];
    $data = $request_param["body"];

    $ch = curl_init($url . '?' . norm_query($params));
    var_dump($url . '?' . norm_query($params));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $method);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
    curl_setopt($ch, CURLOPT_HEADER, false);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $ch_headers);
    curl_setopt($ch, CURLINFO_HEADER_OUT, true);
  
    $response = curl_exec($ch);

    //$req_headers = curl_getinfo($ch, CURLINFO_HEADER_OUT);
    //var_dump($req_headers);

    curl_close($ch);

    //var_dump($response);

    return json_decode($response, true);
}

$now = time();
$body = array();

$response_body = request("GET", $now, array(), array(), $AK, $SK, "STSToken", json_encode($body));

print_r($response_body);
