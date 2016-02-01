#include "ofMain.h"
#include "hiredis/hiredis.h"

class ofxRedisGlob : public ofThread {
  public:
    redisContext *rctx;
    redisReply *reply;
    string hostname;
    int port;
    int period;
    long last_update_at = 0;

    vector<string> get_keys;
    map<string,string> get_map;
    map<string,string> set_map;

    void setup(string hostname, int port, vector<string> _get_keys, int _period=10);
    void update();
    void update(ofEventArgs &eventArgs);

    void subscribeThread();

    void threadedFunction() {
      subscribeThread();
    }

    string get_val(string key, string _default="") {
      string res = _default;
      lock();
      if (get_map.count(key))
        res = get_map[key];
      unlock();
      return res;
    }

    template<typename T>
    void set_val(string key, T val)
    {
      set_map[key] = ofToString(val);
    }
};

