# To build:

Download [emscripten](https://kripken.github.io/emscripten-site/index.html). I used portable SDK. You may need to change your `PATH` depending on how/what you install. If you download portable SDK, you can find SDK binaries (e.g. `emmake`) in `$SDK_ROOT/emsdk_portable/emscripten/$VERSION/`. Then run the following:

```shell
hg clone https://bitbucket.org/m2m/dbc2json.js
cd dbc2json.js
emmake make
```

# To use:

After project is built, `dist/main.js` is created which exports a function called `run`:

```c
const char* run();
```

`run` function expects a DBC file at `/file.dbc` (memory based virtual FS). You can run the following in Javascript to create the file:

```js
// Assuming dbcFile is the string content of an actual dbc file
FS.writeFile('/file.dbc', dbcFile);
```

### Example usage from browser:

```html
<script src="main.js"></script>
<script>
  ...
  // Assuming dbcFile is the string content of an actual dbc file
  var parse = Module.cwrap('run', 'string', []);
  FS.writeFile('/file.dbc', dbcFile);
  var output = parse();
  FS.unlink('/file.dbc');
  console.log(output);
</script>
```

# To clean:

```shell
make clean
```

# To force build everything:

```shell
emmake make -B
```

------------------------------

These tools are provided as-is and without warranty or support. They do not constitute part of the Software AG product suite. Users are free to use, fork and modify them, subject to the license agreement. While Software AG welcomes contributions, we cannot guarantee to include every contribution in the master project.
