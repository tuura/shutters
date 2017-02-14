# Shutters - Wake-up equations for Process-windows
This tool reads `markings` extension files (produced by a process windows mining tool), and generates the wakeup condition for each window, as well as the wakeup marking conditions for the places in each window. The tool uses `espresso` logic minimizer for synthesis of the obtained conditions. `ABC` can be also used optionally for their refactorisation.

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

```bash
Options available:
	-a ABC_path:		specify ABC path
	-e Espresso_path:	specify Espresso logic minimizer path
	-h or --help:		print help of the tool
	-p or --positive:	force equations to contain only positive literals
	-v or --version:	print tool version
```

### Results
The following equations have been obtained by running the tool over the files contained inside the `/test` directory. `buck.marking` models the buck controller, the others are the FSMs depicted in the figures of the `Process Windows` article. Note: the places whose wakeup marking conditions are `0` are not shown for brevity. A place `pN` in window `M` is referred to as `wMpN`.

#### Motivational example

![Workcraft screenshot of the motivational example (Figure 1)](https://github.com/allegroCoder/shutters/blob/master/fig/motivational-example.png)

<table>
  <tr>
    <th>Negative literals without ABC</th>
    <th>Positive literals without ABC</th>
    <th>Negative literals with ABC</th>
    <th>Positive literals with ABC</th>
  </tr>
  <tr>
    <td>Marking eqs. win. 1:<br/>w1p0 = (w2p7);<br/>w1p2 = (!w2p7);<br/>w1p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p0 = (w2p7);<br/>w1p2 = (w2p6);<br/>w1p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p0 = w2p7;<br/>w1p2 = !w2p7;<br/>w1p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p0 = w2p7;<br/>w1p2 = w2p6;<br/>w1p3 = 1;</td>
  </tr>
  <tr>
    <td>Marking eqs. win. 2:<br/>w2p6 = (!w1p0);<br/>w2p7 = (w1p0);<br/>w2p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p6 = (w1p2);<br/>w2p7 = (w1p0);<br/>w2p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p6 = !w1p0;<br/>w2p7 = w1p0;<br/>w2p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p6 = w1p2;<br/>w2p7 = w1p0;<br/>w2p8 = 1;</td>
  </tr>
  <tr>
    <td>Eq. window 1:<br/>w1 = (w2p8*!w2p5);</td>
    <td>Eq. window 1:<br/>w1 = (w2p6*w2p8) + (w2p8*w2p7);</td>
    <td>Eq. window 1:<br/>w1 = !w2p5 * w2p8;</td>
    <td>Eq. window 1:<br/>w1 = w2p8 * (w2p7 + w2p6);</td>
  </tr>
  <tr>
    <td>Eq. window 2:<br/>w2 = (!w1p1*w1p3);</td>
    <td>Eq. window 2:<br/>w2 = (w1p3*w1p2) + (w1p3*w1p0);</td>
    <td>Eq. window 2:<br/>w2 = !w1p1 * w1p3;</td>
    <td>Eq. window 2:<br/>w2 = w1p3 * (w1p0 + w1p2);</td>
  </tr>
</table>

#### Arbiter

![Workcraft screenshot of the arbiter (Figure 9)](https://github.com/allegroCoder/shutters/blob/master/fig/arbiter.png)

<table>
  <tr>
    <th>Negative literals without ABC</th>
    <th>Positive literals without ABC</th>
    <th>Negative literals with ABC</th>
    <th>Positive literals with ABC</th>
  </tr>
  <tr>
    <td>Marking eqs. win. 1:<br/>w1p0 = (!w2p9);<br/>w1p1 = (w2p9);<br/>w1p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p0 = (w2p8);<br/>w1p1 = (w2p9);<br/>w1p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p0 = !w2p9;<br/>w1p1 = w2p9;<br/>w1p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p0 = w2p8;<br/>w1p1 = w2p9;<br/>w1p3 = 1;</td>
  </tr>
  <tr>
    <td>Marking eqs. win. 2:<br/>w2p5 = (!w1p4);<br/>w2p6 = (w1p4);<br/>w2p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p5 = (w1p3);<br/>w2p6 = (w1p4);<br/>w2p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p5 = !w1p4;<br/>w2p6 = w1p4;<br/>w2p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p5 = w1p3;<br/>w2p6 = w1p4;<br/>w2p8 = 1;</td>
  </tr>
  <tr>
    <td>Eq. window 1:<br/>w1 = (w2p5);</td>
    <td>Eq. window 1:<br/>w1 = (w2p5);</td>
    <td>Eq. window 1:<br/>w1 = w2p5;</td>
    <td>Eq. window 1:<br/>w1 = w2p5;</td>
  </tr>
  <tr>
    <td>Eq. window 2:<br/>w2 = (w1p0);</td>
    <td>Eq. window 2:<br/>w2 = (w1p0);</td>
    <td>Eq. window 2:<br/>w2 = w1p0;</td>
    <td>Eq. window 2:<br/>w2 = w1p0;</td>
  </tr>
</table>

#### OR causality

![Workcraft screenshot of the OR causality (Figure 10)](https://github.com/allegroCoder/shutters/blob/master/fig/or-causality.png)

<table>
  <tr>
    <th>Negative literals without ABC</th>
    <th>Positive literals without ABC</th>
    <th>Negative literals with ABC</th>
    <th>Positive literals with ABC</th>
  </tr>
  <tr>
    <td>Marking eqs. win. 1:<br/>w1.p0 = 1;<br/>w1.p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1.p0 = 1;<br/>w1.p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1.p0 = 1;<br/>w1.p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1.p0 = 1;<br/>w1.p3 = 1;</td>
  </tr>
  <tr>
    <td>Marking eqs. win. 2:<br/>w2.p5 = 1;<br/>w2.p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2.p5 = 1;<br/>w2.p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2.p5 = 1;<br/>w2.p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2.p5 = 1;<br/>w2.p8 = 1;</td>
  </tr>
  <tr>
    <td>Eq. window 1:<br/>w1 = (!w2.p9);</td>
    <td>Eq. window 1:<br/>w1 = (w2.p8);</td>
    <td>Eq. window 1:<br/>w1 = !w2.p9;</td>
    <td>Eq. window 1:<br/>w1 = w2.p8;</td>
  </tr>
  <tr>
    <td>Eq. window 2:<br/>w2 = (!w1.p4);</td>
    <td>Eq. window 2:<br/>w2 = (w1.p3);</td>
    <td>Eq. window 2:<br/>w2 = !w1.p4;</td>
    <td>Eq. window 2:<br/>w2 = w1.p3;</td>
  </tr>
</table>

#### Buck controller

![Workcraft screenshot of the Buck controller)](https://github.com/allegroCoder/shutters/blob/master/fig/buck-controller.png)
![Workcraft screenshot of the windows derived](https://github.com/allegroCoder/shutters/blob/master/fig/buck-windows.png)

<table>
  <tr>
    <th>Negative literals without ABC</th>
    <th>Positive literals without ABC</th>
    <th>Negative literals with ABC</th>
    <th>Positive literals with ABC</th>
  </tr>
  <tr>
    <td>Marking eqs. win. 1:<br/>w1.p1 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1.p1 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1.p1 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1.p1 = 1;</td>
  </tr>
  <tr>
    <td>Marking eqs. win. 2:<br/>w2.p1 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2.p1 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2.p1 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2.p1 = 1;</td>
  </tr>
  <tr>
    <td>Marking eqs. win. 3:<br/>w3.p3 = 1;</td>
    <td>Marking eqs. win. 3:<br/>w3.p3 = 1;</td>
    <td>Marking eqs. win. 3:<br/>w3.p3 = 1;</td>
    <td>Marking eqs. win. 3:<br/>w3.p3 = 1;</td>
  </tr>
  <tr>
    <td>Eq. window 1:<br/>w1 = (w2.p1*w3.p3);</td>
    <td>Eq. window 1:<br/>w1 = (w2.p1*w3.p3);</td>
    <td>Eq. window 1:<br/>w1 = w2.p1 * w3.p3;</td>
    <td>Eq. window 1:<br/>w1 = w2.p1 * w3.p3;</td>
  </tr>
  <tr>
    <td>Eq. window 2:<br/>w2 = (w1.p1*w3.p3);</td>
    <td>Eq. window 2:<br/>w2 = (w1.p1*w3.p3);</td>
    <td>Eq. window 2:<br/>w2 = w1.p1 * w3.p3;</td>
    <td>Eq. window 2:<br/>w2 = w1.p1 * w3.p3;</td>
  </tr>
  <tr>
    <td>Eq. window 3:<br/>w3 = (w1.p1*w2.p1);</td>
    <td>Eq. window 3:<br/>w3 = (w1.p1*w2.p1);</td>
    <td>Eq. window 3:<br/>w3 = w1.p1 * w2.p1;</td>
    <td>Eq. window 3:<br/>w3 = w1.p1 * w2.p1;</td>
  </tr>
</table>
