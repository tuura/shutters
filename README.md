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

#### Figure 1
<table>
  <tr>
    <th>Negative literals without ABC</th>
    <th>Positive literals without ABC</th>
    <th>Negative literals with ABC</th>
    <th>Positive literals with ABC</th>
  </tr>
  <tr>
    <td>Marking eqs. win. 1:<br/>w1p1 = (!w2p3);<br/>w1p2 = (w2p3);<br/>w1p5 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p1 = (w2p2);<br/>w1p2 = (w2p3);<br/>w1p5 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p1 = !w2p3;<br/>w1p2 = w2p3;<br/>w1p5 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p1 = w2p2;<br/>w1p2 = w2p3;<br/>w1p5 = 1;</td>
  </tr>
  <tr>
    <td>Marking eqs. win. 2:<br/>w2p2 = (!w1p2);<br/>w2p3 = (w1p2);<br/>w2p5 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p2 = (w1p1);<br/>w2p3 = (w1p2);<br/>w2p5 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p2 = !w1p2;<br/>w2p3 = w1p2;<br/>w2p5 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p2 = w1p1;<br/>w2p3 = w1p2;<br/>w2p5 = 1;</td>
  </tr>
  <tr>
    <td>Eq. window 1:<br/>w1 = (!w2p1&w2p5);</td>
    <td>Eq. window 1:<br/>w1 = (w2p5*w2p2) + (w2p5*w2p3);</td>
    <td>Eq. window 1:<br/>w1 = (w2p5*w2p2) + (w2p5*w2p3);</td>
    <td>Eq. window 1:<br/>w1 = w2p5 * (w2p3 + w2p2);</td>
  </tr>
  <tr>
    <td>Eq. window 2:<br/>w2 = (w1p5&!w1p3);</td>
    <td>Eq. window 2:<br/>w2 = (w1p1*w1p5) + (w1p5*w1p2);</td>
    <td>Eq. window 2:<br/>w2 = !w1p3 * w1p5;</td>
    <td>Eq. window 2:<br/>w2 = w1p5 * (w1p2 + w1p1);</td>
  </tr>
</table>

#### Figure 9
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

#### Figure 10
<table>
  <tr>
    <th>Negative literals without ABC</th>
    <th>Positive literals without ABC</th>
    <th>Negative literals with ABC</th>
    <th>Positive literals with ABC</th>
  </tr>
  <tr>
    <td>Marking eqs. win. 1:<br/>w1p0 = 1;<br/>w1p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p0 = 1;<br/>w1p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p0 = 1;<br/>w1p3 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p0 = 1;<br/>w1p3 = 1;</td>
  </tr>
  <tr>
    <td>Marking eqs. win. 2:<br/>w2p5 = 1;<br/>w2p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p5 = 1;<br/>w2p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p5 = 1;<br/>w2p8 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p5 = 1;<br/>w2p8 = 1;</td>
  </tr>
  <tr>
    <td>Eq. window 1:<br/>w1 = (w2p5*!w2p9);</td>
    <td>Eq. window 1:<br/>w1 = (w2p5*w2p8);</td>
    <td>Eq. window 1:<br/>w1 = !w2p9 * w2p5;</td>
    <td>Eq. window 1:<br/>w1 = w2p5 * w2p8;</td>
  </tr>
  <tr>
    <td>Eq. window 2:<br/>w2 = (w1p0*!w1p4);</td>
    <td>Eq. window 2:<br/>w2 = (w1p0*w1p3);</td>
    <td>Eq. window 2:<br/>w2 = !w1p4 * w1p0;</td>
    <td>Eq. window 2:<br/>w2 = w1p0 * w1p3;</td>
  </tr>
</table>

#### Buck controller
<table>
  <tr>
    <th>Negative literals without ABC</th>
    <th>Positive literals without ABC</th>
    <th>Negative literals with ABC</th>
    <th>Positive literals with ABC</th>
  </tr>
  <tr>
    <td>Marking eqs. win. 1:<br/>w1p1 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p1 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p1 = 1;</td>
    <td>Marking eqs. win. 1:<br/>w1p1 = 1;</td>
  </tr>
  <tr>
    <td>Marking eqs. win. 2:<br/>w2p1 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p1 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p1 = 1;</td>
    <td>Marking eqs. win. 2:<br/>w2p1 = 1;</td>
  </tr>
  <tr>
    <td>Marking eqs. win. 3:<br/>w3p3 = 1;</td>
    <td>Marking eqs. win. 3:<br/>w3p3 = 1;</td>
    <td>Marking eqs. win. 3:<br/>w3p3 = 1;</td>
    <td>Marking eqs. win. 3:<br/>w3p3 = 1;</td>
  </tr>
  <tr>
    <td>Eq. window 1:<br/>w1 = (w2p1*w3p3);</td>
    <td>Eq. window 1:<br/>w1 = (w2p1*w3p3);</td>
    <td>Eq. window 1:<br/>w1 = w2p1 * w3p3;</td>
    <td>Eq. window 1:<br/>w1 = w2p1 * w3p3;</td>
  </tr>
  <tr>
    <td>Eq. window 2:<br/>w2 = (w1p1*w3p3);</td>
    <td>Eq. window 2:<br/>w2 = (w1p1*w3p3);</td>
    <td>Eq. window 2:<br/>w2 = w1p1 * w3p3;</td>
    <td>Eq. window 2:<br/>w2 = w1p1 * w3p3;</td>
  </tr>
  <tr>
    <td>Eq. window 3:<br/>w3 = (w1p1*w2p1);</td>
    <td>Eq. window 3:<br/>w3 = (w1p1*w2p1);</td>
    <td>Eq. window 3:<br/>w3 = w1p1 * w2p1;</td>
    <td>Eq. window 3:<br/>w3 = w1p1 * w2p1;</td>
  </tr>
</table>
