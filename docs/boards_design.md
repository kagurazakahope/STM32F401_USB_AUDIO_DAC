<table>
    <tr>
        <td>Interface</td>
        <td>Pinout</td>
        <td>Description</td>
    </tr>
    <tr><td rowspan="19">MCU-DAC</td>
        <td>+5V</td><td>USB VBus</td></tr>
    <tr><td>VREF+</td><td>3.3V Analog support from MCU board</td></tr>
    <tr><td>B13</td><td>I2S Bit Clock</td></tr>
    <tr><td>B15</td><td>I2S Data</td></tr>
    <tr><td>B12</td><td>I2S LR Clock</td></tr>
    <tr><td>GND</td><td>Digital Ground</td></tr>
    <tr><td>A8</td><td>MUTE control output</td></tr>
    <tr><td>A11</td><td>USB D-</td></tr>
    <tr><td>A12</td><td>USB D+</td></tr>
    <tr><td>A2</td><td>Serial debug TX</td></tr>
    <tr><td>A3</td><td>Serial debug RX</td></tr>
    <tr><td>A0</td><td>Trigger endpoint feedback</td></tr>
    <tr><td>B3</td><td>LED a</td></tr>
    <tr><td>B4</td><td>LED b</td></tr>
    <tr><td>B5</td><td>LED c</td></tr>
    <tr><td>B6</td><td>LED d</td></tr>
    <tr><td>B7</td><td>LED e</td></tr>
    <tr><td>B8</td><td>LED f</td></tr>
    <tr><td>B9</td><td>LED g</td></tr>
    <tr><td rowspan="19">DAC-AMP</td>
        <td>LOUT</td><td>Left output</td>
    <tr><td>ROUT</td><td>Right output</td></tr>
    <tr><td>AGND</td><td>Analog ground</td></tr>
    <tr><td>3V3</td><td>3.3V Analog support from MCU board. Need a jumper to VCC if AMP board is not used.</td></tr>
    <tr><td>VCC</td><td>Support to DAC chip. Either jumper from 3V3 or connected to AMP board with external power supply.</td></tr>
    <tr><td>V_External</td><td>3.3V support from AMP board</td></tr>
    <tr><td>MUTE</td><td>MUTE control from MCU board</td></tr>
    <tr><td>ZERO</td><td>ZERO indicator from DAC</td></tr>
</table>
