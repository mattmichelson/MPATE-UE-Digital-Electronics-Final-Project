# MIDI Clap Rhythm Sequencer
You're there with your DAW open, ready to make music. You think of a drum rhythm in your head but 30 minutes pass by and you're still programming the perfect rhythm! Here is where my project, the MIDI clap-rhythm sequencer, comes in handy. You no longer have to go through the tedious process of dragging and clicking to get the perfect rhythm. My project allows you to record rhythms by clapping into an adjustable 16-step sequencer. Want to store multiple rhythms? My project has 16 channels that you could easily write to. When you want to send these rhythms to your DAW, simply stop recording, press record on your DAW, and consider the job done. Do you want to make the rhythm faster or slower? Just turn the blue knob. You don't like what you recorded? Press the top right button to clear the current channel or the bottom middle button to clear all channels. Do you want to play your rhythm in reverse? Just flip the switch! The MIDI clap-rhythm sequencer would be a very useful asset in any studio.

<h1>The details</h1>
<h2>External components</h2>
<p>In this project, I used the Teensy 3.5, a micro controller, which was wired to a breadboard. The breadboard aids in the communication between the teensy and the code being programmed to it, and the external components in this project. The external components used includes a potentiometer to control the tempo, one button to record the claps, 1 button to reset the channel, one button to reset all the channels, one button to go to the next channel, one button to go to the previous channel, a switch to control forwards and backwards, a potentiometer to control the tempo, a sound detector to detect claps, and a seven segment display to display the channel. (See code for specific pins).</p>
<p>The seven segment display has four pins: CLK, OUT, VCC(3.5V), and GND. The clock and output were set to pins 2 and 3 on it's data sheet, so I kept it that way.</p>
<p>The sound detector had three outputs: VCC, GND, and OUT. Since I have the power rails on the breadboard running 3V and it requires 5V, I had to connect it directly into the 5V pin on the teensy. I initialized it with the other buttons in the button array as buttonPins [0]. (See code).</p>
<h2>Source Code with components</h2>
<p>The Teensy source code accesses the Serial and MIDI libraries. All the input and output digital pins are configured in the setup() function.</p>
<h3> </h3>
<h3>The LEDs and the Sequencer</h3>
<p>First, I started with making my program step through the four LEDs, which would send 3.5V to each LED for a specified amount of time. In order to do this, in Arduino, you must first set the LED pins as outputs, make code that sends these 3.5V to a specified pin, then send a delay of a specified amount of time, so that pin stays on. Then, I sent that pin off for a certain amount of time, and repeated the process for each pin. I eventually replaced this with code that made the LEDs step more smoothly. If the program reached the point where it was time to light up the following LED, it would turn off the current LED, increment to the next step, set the last time it stepped to the current time of the program (time travel hehehe), and finally turn on the next LED. When this record button is pressed a second time, it would send the steps selected as midi notes that a DAW could recognize. I programmed it so that there is one midi note per channel. This piece of code also happens to allow the potentiometer, which controls the tempo of the sequencer, respond immediately, instead of waiting for the first iteration of the sequence to conclude. Then, I expanded the code to include 16 LEDs, which was just a matter of replacing the number of LEDs. The sequencer works by utilizing a 2D array: one array defines the actual stepping and the other array defines the channels. The LEDs use this array to know whether to turn on or off.</p>
<h3>Tempo and potentiometer</h3>
<p>I approached controlling the tempo by having its value be read by the delay. The potentiometer is an analog component, so it needed this syntax:</p>
<p><code> delay (analogRead(A13));</code></p>
<p>Then, I was able to simply concatenate the tempo when it compared if the program time was more than the last program time, like so:</p>
<p><code>if(millis() &gt; lastStepTime + tempo) { . . .<br />
</code></p>
<p><img class="alignnone  wp-image-139" src="https://mattmichelsondigitallab.files.wordpress.com/2018/12/Screen-Shot-2018-12-12-at-12.11.15-PM.png" alt="Screen Shot 2018-12-12 at 12.11.15 PM" width="89" height="210" /></p>
<h3>Switch</h3>
<p>The switch allows you to decide if you want the sequence to move forwards or backwards. If it is switched on, the program will increment the current step by 1 for the next step; however, if it is switched off, the step will decrement in the opposite direction. Since the LEDs are reading the current step, the LEDs will respond to the changes in the current step variable.(See code for pinNumber).</p>
<p><img class="alignnone  wp-image-121" src="https://mattmichelsondigitallab.files.wordpress.com/2018/12/screen-shot-2018-12-12-at-9-45-47-am.png" alt="Screen Shot 2018-12-12 at 9.45.47 AM" width="176" height="200" /></p>
<h3>Buttons</h3>
<p>In order to get the buttons to work properly, I had to make sure my program was checking to see if they were pressed at all times. I wrote code for all of the buttons that allowed them to function immediately where it only turns on if the previous button state is low. I use this logic for each of the buttons: record, erase, master erase, next channel, and previous channel.</p>
<p><img class="alignnone  wp-image-132" src="https://mattmichelsondigitallab.files.wordpress.com/2018/12/screen-shot-2018-12-12-at-11-31-45-am1.png" alt="Screen Shot 2018-12-12 at 11.31.45 AM" width="176" height="169" /></p>
<pre>  <span style="color: #5e6d03;">for</span> <span style="color: #000000;">(</span><span style="color: #00979c;">int</span> <span style="color: #000000;">i</span> <span style="color: #434f54;">=</span> <span style="color: #000000;">0</span><span style="color: #000000;">;</span> <span style="color: #000000;">i</span> <span style="color: #434f54;">&lt;</span> <span style="color: #000000;">4</span><span style="color: #000000;">;</span> <span style="color: #000000;">i</span><span style="color: #434f54;">++</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>
    <span style="color: #000000;">lastButtonStates</span><span style="color: #000000;">[</span><span style="color: #000000;">i</span><span style="color: #000000;">]</span> <span style="color: #434f54;">=</span> <span style="color: #000000;">buttonStates</span> <span style="color: #000000;">[</span><span style="color: #000000;">i</span><span style="color: #000000;">]</span><span style="color: #000000;">;</span>
    <span style="color: #000000;">buttonStates</span> <span style="color: #000000;">[</span><span style="color: #000000;">i</span><span style="color: #000000;">]</span> <span style="color: #434f54;">=</span> <span style="color: #d35400;">digitalRead</span><span style="color: #000000;">(</span><span style="color: #000000;">buttonPins</span> <span style="color: #000000;">[</span><span style="color: #000000;">i</span><span style="color: #000000;">]</span><span style="color: #000000;">)</span><span style="color: #000000;">;</span>
  <span style="color: #000000;">}</span>
</pre>
<h4> </h4>
<h4>Microphone</h4>
<p>Before I added the microphone, I had a button in its place. So it basically functions like the rest of the buttons in the code except for the addition of a delay in the times it checks for input, which I included in an attempt to prevent the sound detector from detecting sustained sounds and only recognize the hard transients of a clap.</p>
<pre>  <span style="color: #5e6d03;">if</span> <span style="color: #000000;">(</span><span style="color: #d35400;">millis</span><span style="color: #000000;">(</span><span style="color: #000000;">)</span> <span style="color: #434f54;">&gt;</span> <span style="color: #000000;">lastRecordTime</span> <span style="color: #434f54;">+</span> <span style="color: #000000;">50</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>   <span style="color: #434f54;">//if its time to go to the next step...</span>
    <span style="color: #434f54;">//mic button</span>
    <span style="color: #5e6d03;">if</span> <span style="color: #000000;">(</span><span style="color: #000000;">buttonStates</span> <span style="color: #000000;">[</span><span style="color: #000000;">0</span><span style="color: #000000;">]</span> <span style="color: #434f54;">==</span> <span style="color: #00979c;">HIGH</span> <span style="color: #434f54;">&amp;&amp;</span> <span style="color: #000000;">lastButtonStates</span> <span style="color: #000000;">[</span><span style="color: #000000;">0</span><span style="color: #000000;">]</span> <span style="color: #434f54;">==</span> <span style="color: #00979c;">LOW</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>
      <span style="color: #5e6d03;">if</span> <span style="color: #000000;">(</span><span style="color: #000000;">on</span><span style="color: #000000;">[</span><span style="color: #000000;">channelDisplayed</span> <span style="color: #434f54;">-</span> <span style="color: #000000;">1</span><span style="color: #000000;">]</span><span style="color: #000000;">[</span><span style="color: #000000;">currentStep</span><span style="color: #000000;">]</span> <span style="color: #434f54;">==</span> <span style="color: #00979c;">false</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>
        <span style="color: #000000;">on</span><span style="color: #000000;">[</span><span style="color: #000000;">channelDisplayed</span> <span style="color: #434f54;">-</span> <span style="color: #000000;">1</span><span style="color: #000000;">]</span><span style="color: #000000;">[</span><span style="color: #000000;">currentStep</span><span style="color: #000000;">]</span> <span style="color: #434f54;">=</span> <span style="color: #00979c;">true</span><span style="color: #000000;">;</span>
        <span style="color: #000000;">lastRecordTime</span> <span style="color: #434f54;">=</span> <span style="color: #d35400;">millis</span><span style="color: #000000;">(</span><span style="color: #000000;">)</span><span style="color: #000000;">;</span>
      <span style="color: #000000;">}</span>
    <span style="color: #000000;">}</span>
</pre>
<h4><img class="fancybox-image" src="https://tinkersphere.com/4111-thickbox_default/microphone-sound-detector-arduino-compatible.jpg" alt="" width="150" height="150" /></h4>
<p><span style="color: #ff0000;">EDIT: I changed the code to reflect the replacement of the digital sound detector to the analog sound detector. The code below reflects these changes.</span></p>
<pre>  <span style="color: #5e6d03;">if</span> <span style="color: #000000;">(</span><span style="color: #d35400;">millis</span><span style="color: #000000;">(</span><span style="color: #000000;">)</span> <span style="color: #434f54;">&gt;</span> <span style="color: #000000;">lastRecordTime</span> <span style="color: #434f54;">+</span> <span style="color: #000000;">50</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>   <span style="color: #434f54;">//if its time to go to the next step...</span>
    <span style="color: #434f54;">//mic button</span>
     <span style="color: #5e6d03;">if</span> <span style="color: #000000;">(</span><span style="color: #d35400;">analogRead</span><span style="color: #000000;">(</span><span style="color: #00979c;">A17</span><span style="color: #000000;">)</span> <span style="color: #434f54;">&gt;</span> <span style="color: #000000;">thresholdVar</span> <span style="color: #434f54;">&amp;&amp;</span> <span style="color: #000000;">lastMicState</span> <span style="color: #434f54;">==</span> <span style="color: #00979c;">LOW</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>
      <span style="color: #5e6d03;">if</span> <span style="color: #000000;">(</span><span style="color: #000000;">on</span><span style="color: #000000;">[</span><span style="color: #000000;">channelDisplayed</span> <span style="color: #434f54;">-</span> <span style="color: #000000;">1</span><span style="color: #000000;">]</span><span style="color: #000000;">[</span><span style="color: #000000;">currentStep</span><span style="color: #000000;">]</span> <span style="color: #434f54;">==</span> <span style="color: #00979c;">false</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>
        <span style="color: #000000;">on</span><span style="color: #000000;">[</span><span style="color: #000000;">channelDisplayed</span> <span style="color: #434f54;">-</span> <span style="color: #000000;">1</span><span style="color: #000000;">]</span><span style="color: #000000;">[</span><span style="color: #000000;">currentStep</span><span style="color: #000000;">]</span> <span style="color: #434f54;">=</span> <span style="color: #00979c;">true</span><span style="color: #000000;">;</span>
        <span style="color: #000000;">lastRecordTime</span> <span style="color: #434f54;">=</span> <span style="color: #d35400;">millis</span><span style="color: #000000;">(</span><span style="color: #000000;">)</span><span style="color: #000000;">;</span>
      <span style="color: #000000;">}</span>
    <span style="color: #000000;">}</span>
  <span style="color: #000000;">}</span>

</pre>
<h4>Midi</h4>
<p>The arduino library contains code that allows you manipulate MIDI messages. I incorporated the 'send midi note' function in this library into my code. I made a separate function that behaved like the sequencer that selected the current step, but instead of turning the selected LED on, it turns a MIDI note on when the selected step is found.</p>
<h4>7 Segment Display</h4>
<p>The seven segment display displays the channel the sequencer is currently on. The seven segment display utilizes an external library that was downloaded from here:</p>
<p><a href="https://playground.arduino.cc/Main/TM1637">https://playground.arduino.cc/Main/TM1637</a></p>
<p>This library contains additional code that can be accessed by including it in your source code. It also conveyed the specific syntax I needed to use in order to send data to the display. The current channel displays like this: if the next channel or previous channel button was pressed and the current channel was incremented or decremented, then I need to send the current channel number to the display. For example:</p>
<pre>    <span style="color: #5e6d03;">if</span> <span style="color: #000000;">(</span><span style="color: #000000;">prevChannelButtonState</span> <span style="color: #434f54;">==</span> <span style="color: #00979c;">HIGH</span> <span style="color: #434f54;">&amp;&amp;</span> <span style="color: #000000;">lastPrevChannelButtonState</span> <span style="color: #434f54;">==</span> <span style="color: #00979c;">LOW</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>
      <span style="color: #434f54;">//Serial.println("Current channel is:" channelDisplayed);</span>

      <span style="color: #434f54;">--</span><span style="color: #000000;">channelDisplayed</span><span style="color: #000000;">;</span>
      <span style="color: #5e6d03;">if</span> <span style="color: #000000;">(</span><span style="color: #000000;">channelDisplayed</span> <span style="color: #434f54;">&lt;</span> <span style="color: #000000;">1</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>
        <span style="color: #000000;">channelDisplayed</span> <span style="color: #434f54;">=</span> <span style="color: #000000;">16</span><span style="color: #000000;">;</span>
      <span style="color: #000000;">}</span>

      <span style="color: #5e6d03;">if</span> <span style="color: #000000;">(</span><span style="color: #000000;">channelDisplayed</span> <span style="color: #434f54;">==</span> <span style="color: #000000;">1</span><span style="color: #000000;">)</span> <span style="color: #000000;">{</span>
        <span style="color: #d35400;">display</span><span style="color: #434f54;">.</span><span style="color: #000000;">showNumberHexEx</span><span style="color: #000000;">(</span><span style="color: #000000;">0xc1</span><span style="color: #000000;">)</span><span style="color: #000000;">;</span>
      <span style="color: #000000;">}
</span>//...etc.</pre>
<p><img class="fancybox-image" src="https://tinkersphere.com/2439-thickbox_default/4-digit-seven-segment-display-module.jpg" alt="" width="128" height="128" /></p>
<h2>List of parts:</h2>
<ul>
<li>7 segment display
<ul>
<li>(<a href="https://tinkersphere.com/segmented-number-leds/1355-4-digit-seven-segment-display-module.html">https://tinkersphere.com/segmented-number-leds/1355-4-digit-seven-segment-display-module.html</a>)</li>
</ul>
</li>
<li>Analog sound detector
<ul>
<li>(<a href="https://tinkersphere.com/arduino-compatible-components/1648-electret-microphone-amplifier-max9812.htm">https://tinkersphere.com/arduino-compatible-components/1648-electret-microphone-amplifier-max9812.htm</a>l)</li>
</ul>
</li>
</ul>
<p> </p>

</pre>
