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
  ofAddListener(ofEvents().update, this, &ofxRedisGlob::update, OF_EVENT_ORDER_AFTER_APP);  
}

void ofxRedisGlob::update(ofEventArgs &eventArgs) {
  update();
}

void ofxRedisGlob::update() {
  if (ofGetElapsedTimeMillis() - last_update_at < period) return;

  reply = (redisReply *)redisCommand(rctx,"MGET %s", ofJoinString(get_keys, " ").c_str());
  if (rctx->err) {
    ofLog() << "ERR!";
  }
  for (int i=0; i<get_keys.size(); i++) {
    if (reply->element[i]->str != NULL) {
      get_map[get_keys[i]] = reply->element[i]->str;
    }
  }
  freeReplyObject(reply);

  if (set_map.size()) {
    stringstream ss;
    ss << "MSET ";
    for (auto& kv : set_map) {
      ss << " " << kv.first << " " << kv.second;
    }

    string cmd = ss.str();
    reply = (redisReply *)redisCommand(rctx, cmd.c_str());
    freeReplyObject(reply);
  }
  set_map.clear();

  last_update_at = ofGetElapsedTimeMillis();
}

