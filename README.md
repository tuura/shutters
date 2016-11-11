# Wake-up equations for Process-windows
This plugin reads `markings` extension files (`process-windows` application), and generate the wakeup marking conditions for the places and for the windows present, using `Espresso` logic minimizer.

### How to install the tool
Clone the repository and run the Makefile.
```bash
$ git clone https://github.com/allegroCoder/wakeup-eq.git # clone the repository
$ make # compile and build the tool
```
Run `make help` for displaying the Makefile commands.

### How to use the tool
```bash
$ ./wakeup-eq [file.markings] -e [espresso_path]
```
Run `./wakeup-eq --help` for the help of the tool.

### Results
The followind equations have been obtained by running the tool over the files contained inside the `/test` directory. `buck.marking` models the buck controller, the others are the FSMs depicted in the figures of the `Process Windows` article. Notice: the missing places in the wakeup marking conditions are calculated to be not marked (`0` condition).

#### fig1
```text
Wakeup marking conditions for Window 1:
w1p1 = (!w2p3);
w1p5 = 1;
w1p2 = (w2p3);
Wakeup marking conditions for Window 2:
w2p5 = 1;
w2p2 = (!w1p2);
w2p3 = (w1p2);
Wakeup condition for Window 1:
w1 = (!w2p1&w2p5);
Wakeup condition for Window 2:
w2 = (w1p5&!w1p3);
```

#### fig3
```text
Wakeup marking conditions for Window 1:
w1p1 = (w2p4);
w1p4 = 1;
w1p2 = (!w2p4);
Wakeup marking conditions for Window 2:
w2p1 = (w1p4);
w2p4 = 1;
w2p2 = (!w1p4);
Wakeup condition for Window 1:
w1 = (w2p1);
Wakeup condition for Window 2:
w2 = (w1p1);
```

#### fig4
```text
Wakeup marking conditions for Window 1:
w1p3 = 1;
w1p5 = 1;
Wakeup marking conditions for Window 2:
w2p1 = 1;
Wakeup condition for Window 1:
w1 = (w2p1);
Wakeup condition for Window 2:
w2 = (w1p3&!w1p4);
```

#### buck
```text
Wakeup marking conditions for Window 1:
w1p1 = 1;
Wakeup marking conditions for Window 2:
w2p1 = 1;
Wakeup marking conditions for Window 3:
w3p3 = 1;
Wakeup condition for Window 1:
w1 = (w2p1&w3p3);
Wakeup condition for Window 2:
w2 = (w1p1&w3p3);
Wakeup condition for Window 3:
w3 = (w1p1&w2p1);
```
