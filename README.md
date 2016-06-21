To get started:

Download [emscripten](https://kripken.github.io/emscripten-site/index.html). I used portable SDK. You may need to change your `PATH` depending on how/what you install. If you download portable SDK, you can find SDK binaries (e.g. `emmake`) in `$SDK_ROOT/emsdk_portable/emscripten/$VERSION/`. Then run the following:

```shell
hg clone https://bitbucket.org/m2m/dbc2json.js
cd dbc2json.js
emmake make
```

To clean:

```shell
make clean
```

To force build (in case stuff gets out of sync):

```shell
emmake make -B
```
