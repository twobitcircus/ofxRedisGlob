# ofxRedisGlob

This is a quick and easy addon for using a redis server to get and set global state across multiple pieces of software.  Values you set with `set_val` will automatically propogated to the server and keys you designate during `setup()` will automatically be kept up to date.  All at the specified period.

There's an easy-to-access redis context in this object which you can easily use for other stuff too

```
#include "ofxRedisGlob.h"

ofxRedisGlob glob;

void ofApp::setup() {
  vector<string> get_keys;
  get_keys.push_back("monkeys", "dogs");

  glob.setup("127.0.0.1", 6379, get_keys);
  // glob.update() is automatically called for you
}

void ofApp::update() {
  string monkeys = glob.get_val("monkeys");
  glob.set_val("something", 123); // second argument is automatically cast to a string
}
```

