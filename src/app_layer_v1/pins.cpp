Content-Type: text/enriched
Text-Width: 70

<italic><x-color><param>#586e75</param>/*</x-color></italic><italic><x-color><param>#586e75</param>
 * Teensy4 IOIO-OTG Project
 * This project is a port of the IOIO-OTG firmware to the Teensy 4.x platform.
 * Portions of this code may have been rewritten or replaced entirely.
 *
 * Copyright 2011 Ytai Ben-Tsvi. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ARSHAN POURSOHI OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied.
 *
</x-color></italic><italic><x-color><param>#586e75</param> */</x-color></italic>


<x-color><param>olive drab</param>#include</x-color> <x-color><param>dark turquoise</param><<assert.h></x-color>


<x-color><param>olive drab</param>#include</x-color> <x-color><param>dark turquoise</param>"pins.h"</x-color>


<x-color><param>olive drab</param>#define</x-color> <x-color><param>#b58900</param>ARRAY_SIZE</x-color>(<x-color><param>#6c71c4</param>x</x-color>) (<x-color><param>#859900</param>sizeof</x-color>(x) / <x-color><param>#859900</param>sizeof</x-color>(x[0]))
<x-color><param>olive drab</param>#define</x-color> <x-color><param>#6c71c4</param>SFR</x-color> <x-color><param>#859900</param>volatile</x-color> <x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color>


<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNENB</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNENC</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNEND</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNENE</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNENF</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNENG</x-color> = 0x0000;

<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNFORCEB</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNFORCEC</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNFORCED</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNFORCEE</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNFORCEF</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNFORCEG</x-color> = 0x0000;

<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNBACKUPB</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNBACKUPC</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNBACKUPD</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNBACKUPE</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNBACKUPF</x-color> = 0x0000;
<x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>CNBACKUPG</x-color> = 0x0000;


<x-color><param>#859900</param>typedef</x-color> <x-color><param>#859900</param>struct</x-color> {
  <x-color><param>#268bd2</param>SFR</x-color>* <x-color><param>#6c71c4</param>tris</x-color>;
  <x-color><param>#268bd2</param>SFR</x-color>* <x-color><param>#6c71c4</param>ansel</x-color>;
  <x-color><param>#268bd2</param>SFR</x-color>* <x-color><param>#6c71c4</param>port</x-color>;
  <x-color><param>#268bd2</param>SFR</x-color>* <x-color><param>#6c71c4</param>lat</x-color>;
  <x-color><param>#268bd2</param>SFR</x-color>* <x-color><param>#6c71c4</param>odc</x-color>;
  <x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color>* <x-color><param>#6c71c4</param>fake_cnen</x-color>;
  <x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color>* <x-color><param>#6c71c4</param>cn_backup</x-color>;
  <x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color>* <x-color><param>#6c71c4</param>cn_force</x-color>;
  <x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pos_mask</x-color>;
  <x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>neg_mask</x-color>;
  <x-color><param>#268bd2</param>PORT</x-color> <x-color><param>#6c71c4</param>name</x-color>;
  <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>nbit</x-color>;
} <x-color><param>#268bd2</param>PORT_INFO</x-color>;


<x-color><param>olive drab</param>#define</x-color> <x-color><param>#b58900</param>MAKE_PORT_INFO</x-color>(<x-color><param>#6c71c4</param>port</x-color>, <x-color><param>#6c71c4</param>num</x-color>) {             \
    &TRIS##port,                                \
      &ANS##port,                               \
      &PORT##port,                              \
      &LAT##port,                               \
      &ODC##port,                               \
      &CNEN##port,                              \
      &CNBACKUP##port,                          \
      &CNFORCE##port,                           \
      (1 <<<< num),                               \
      ~(1 <<<< num),                              \
      PORT_##port,                              \
      num                                       \
      }

<x-color><param>#859900</param>typedef</x-color> <x-color><param>#859900</param>struct</x-color> {
  <x-color><param>#268bd2</param>SFR</x-color> *<x-color><param>#6c71c4</param>cnen</x-color>;
  <x-color><param>#268bd2</param>SFR</x-color> *<x-color><param>#6c71c4</param>cnpu</x-color>;
  <x-color><param>#268bd2</param>SFR</x-color> *<x-color><param>#6c71c4</param>cnpd</x-color>;
  <x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pos_mask</x-color>;
  <x-color><param>#268bd2</param>unsigned</x-color> <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>neg_mask</x-color>;
} <x-color><param>#268bd2</param>CN_INFO</x-color>;

<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinSetTris</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>, <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>val</x-color>) {
}


<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinSetAnsel</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>, <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>val</x-color>) {
}


<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinSetLat</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>, <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>val</x-color>) {
}


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinGetPort</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>) {
  <x-color><param>#859900</param>return</x-color> 0;
}

<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinSetOdc</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>, <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>val</x-color>) {
}


<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinSetCnen</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>, <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>cnen</x-color>) {
}


<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinSetCnforce</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>) {
}


<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinSetCnpu</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>, <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>cnpu</x-color>) {
}


<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinSetCnpd</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>, <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>cnpd</x-color>) {
}


<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinSetRpor</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>, <x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>per</x-color>) {
}


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinFromPortB</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>bit</x-color>) {
  <x-color><param>#859900</param>return</x-color> 0; <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>port_to_pin[0][bit];
</x-color></italic>};


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinFromPortC</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>bit</x-color>) {
  <x-color><param>#859900</param>return</x-color> 0; <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>port_to_pin[1][bit];
</x-color></italic>};


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinFromPortD</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>bit</x-color>) {
  <x-color><param>#859900</param>return</x-color> 0; <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>port_to_pin[2][bit];
</x-color></italic>};


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinFromPortE</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>bit</x-color>) {
  <x-color><param>#859900</param>return</x-color> 0; <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>port_to_pin[3][bit];
</x-color></italic>};


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinFromPortF</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>bit</x-color>) {
  <x-color><param>#859900</param>return</x-color> 0; <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>port_to_pin[4][bit];
</x-color></italic>};


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinFromPortG</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>bit</x-color>) {
  <x-color><param>#859900</param>return</x-color> 0; <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>port_to_pin[5][bit];
</x-color></italic>};


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinFromAnalogChannel</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>ch</x-color>) {
  <x-color><param>#859900</param>return</x-color> 0; <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>analog_to_pin[ch];
</x-color></italic>}


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinToAnalogChannel</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>) {
  <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>if (pin << MIN_ANALOG_PIN
</x-color></italic>  <italic><x-color><param>#586e75</param>//     </x-color></italic><italic><x-color><param>#586e75</param>|| pin - MIN_ANALOG_PIN >= ARRAY_SIZE(pin_to_analog)) {
</x-color></italic>  <italic><x-color><param>#586e75</param>//   </x-color></italic><italic><x-color><param>#586e75</param>return -1;
</x-color></italic>  <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>}
</x-color></italic>  <x-color><param>#859900</param>return</x-color> 0; <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>pin_to_analog[pin - MIN_ANALOG_PIN];
</x-color></italic>}


<x-color><param>#268bd2</param>int</x-color> <x-color><param>#b58900</param>PinToRpin</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>) {
  <x-color><param>#859900</param>return</x-color> 0; <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>pin_to_rpin[pin];
</x-color></italic>}


<x-color><param>#268bd2</param>void</x-color> <x-color><param>#b58900</param>PinToPortBit</x-color>(<x-color><param>#268bd2</param>int</x-color> <x-color><param>#6c71c4</param>pin</x-color>, <x-color><param>#268bd2</param>PORT</x-color> *<x-color><param>#6c71c4</param>port</x-color>, <x-color><param>#268bd2</param>int</x-color> *<x-color><param>#6c71c4</param>nbit</x-color>) {
  <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>const PORT_INFO* info = &port_info[pin];
</x-color></italic>  <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>*port = info->name;
</x-color></italic>  <italic><x-color><param>#586e75</param>// </x-color></italic><italic><x-color><param>#586e75</param>*nbit = info->nbit;
</x-color></italic>}
