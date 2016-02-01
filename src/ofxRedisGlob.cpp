#include "ofxRedisGlob.h"

void ofxRedisGlob::setup(string _hostname, int _port, vector<string> _get_keys, int _period) {
  hostname = _hostname;
  port = _port;
  get_keys = _get_keys;
  period = _period;


  struct timeval timeout = { 1, 500000 };
  rctx = redisConnectWithTimeout(hostname.c_str(), port, timeout);
  if (rctx == NULL || rctx->err) {
    if (rctx) {
      printf("Connection error: %s\n", rctx->errstr);
      redisFree(rctx);
    } else {
      printf("Connection error: can't allocate redis context\n");
    }
    // bail out in some way
  }
  ofAddListener(ofEvents().update, this, &ofxRedisGlob::update, OF_EVENT_ORDER_BEFORE_APP);  

  startThread(true, false);
}

void ofxRedisGlob::update(ofEventArgs &eventArgs) {
  update();
}

void ofxRedisGlob::subscribeThread() {
  redisContext *sub_rctx;
  redisReply *sub_reply;

  struct timeval timeout = { 1, 500000 };
  sub_rctx = redisConnectWithTimeout(hostname.c_str(), port, timeout);
  if (sub_rctx == NULL || sub_rctx->err) {
    if (sub_rctx) {
      printf("Connection error: %s\n", sub_rctx->errstr);
      redisFree(sub_rctx);
    } else {
      printf("Connection error: can't allocate redis context\n");
    }
    return;
  }

  string cmd = "SUBSCRIBE " + ofJoinString(get_keys, " ");
  sub_reply = (redisReply *)redisCommand(sub_rctx,cmd.c_str());
  freeReplyObject(sub_reply);
  while(isThreadRunning() && redisGetReply(sub_rctx,(void **)&sub_reply) == REDIS_OK) {
    string channel = sub_reply->element[1]->str;
    string value = sub_reply->element[2]->str;

    lock();
    get_map[channel] = value;
    unlock();
    freeReplyObject(sub_reply);
  }
}

void ofxRedisGlob::update() {
  if (ofGetElapsedTimeMillis() - last_update_at < period) return;

  if (set_map.size()) {
    for (auto& kv : set_map) {
      reply = (redisReply *)redisCommand(rctx, "PUBLISH %s %s", kv.first.c_str(), kv.second.c_str());
      freeReplyObject(reply);
    }
  }
  set_map.clear();
  last_update_at = ofGetElapsedTimeMillis();
}

