#include "ofMain.h"
#include "hiredis/hiredis.h"

class ofxRedisGlob {
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

    string get_val(string key, string _default="") {
      if (get_map.count(key))
        return get_map[key];
      else
        return _default;
    }

    template<typename T>
    void set_val(string key, T val)
    {
      set_map[key] = ofToString(val);
    }
};

