# Shutters - Wake-up equations for Process-windows
This tool reads `markings` extension files (produced by a process windows mining tool), and generates the wakeup condition for each window, as well as the wakeup marking conditions for the places in each window. The tool uses `espresso` logic minimizer for synthesis of the obtained conditions.

### How to install the tool
Clone the repository and run the Makefile.
```bash
$ git clone https://github.com/allegroCoder/shutters.git # clone the repository
$ make # compile and build the tool
```
Run `make help` for displaying the Makefile commands.

### How to use the tool
```bash
$ ./shutters [file.markings] -e [espresso_path]
```
Run `./shutters --help` for the help of the tool.

### Results
The followind equations have been obtained by running the tool over the files contained inside the `/test` directory. `buck.marking` models the buck controller, the others are the FSMs depicted in the figures of the `Process Windows` article. Note: the places whose wakeup marking conditions are `0` are not shown for brevity. A place `pN` in window `M` is referred to as `wMpN`.

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
