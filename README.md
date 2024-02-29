# Arduino RF rolling code keeloq receiver

Arduino RF rolling code keeloq receiver 

Programmer:<br>
Software: https://github.com/ioelectro/hcs-programmer-soft<br>
Hardwer: https://github.com/ioelectro/arduino-hcs-programmer<br>
Doc: https://ioelectro.ir/blog/hcs-programmer/<br>

Keeloq Encoder/Decoder:<br>
Algoritm: https://github.com/liyanboy74/KeeLoq<br>
Doc: https://ioelectro.ir/blog/keeloq/ <br>

just Connect RF 433 or 315 Mhz resiver to pin 2 (INT0) arduino UNO v3.<br>
for correct encoding the encrypted part of received packet we must know the key of keeloq algorithm.

**Pinout**
![How-433MHz-RF-Tx-Rx-Modules-Work-Interface-with-Arduino-UNO](https://github.com/liyanboy74/arduino-rf-rolling-code-keeloq-receiver/assets/64005694/494af9d4-ec15-42f3-8ccd-76c05fbe732f)

![Packet](https://github.com/liyanboy74/arduino-rf-rolling-code-keeloq-receiver/assets/64005694/ac3b46ce-226e-42ef-95df-c9de44f4fd20)

Output Example:
```
Ready
001: fix=27E30EC3 : vr=1 btn=2 ser=7E30EC3 , enc=[B7F8C4B3] --> dec=2EC30762 : btn=2 ovr=3 disc=2C3 C=762 
002: fix=87E30EC3 : vr=0 btn=8 ser=7E30EC3 , enc=[0E58DE70] --> dec=8EC30763 : btn=8 ovr=3 disc=2C3 C=763 
003: fix=47E30EC3 : vr=1 btn=4 ser=7E30EC3 , enc=[4F2A7A4A] --> dec=4EC30764 : btn=4 ovr=3 disc=2C3 C=764 
004: fix=27E30EC3 : vr=1 btn=2 ser=7E30EC3 , enc=[663BD595] --> dec=2EC30765 : btn=2 ovr=3 disc=2C3 C=765 
005: fix=27E30EC3 : vr=1 btn=2 ser=7E30EC3 , enc=[663BD595] --> dec=2EC30765 : btn=2 ovr=3 disc=2C3 C=765 
006: fix=27E30EC3 : vr=1 btn=2 ser=7E30EC3 , enc=[663BD595] --> dec=2EC30765 : btn=2 ovr=3 disc=2C3 C=765 
007: fix=17E30EC3 : vr=0 btn=1 ser=7E30EC3 , enc=[E545BED6] --> dec=1EC30766 : btn=1 ovr=3 disc=2C3 C=766 
008: fix=17E30EC3 : vr=1 btn=1 ser=7E30EC3 , enc=[99C1FFFD] --> dec=1EC30767 : btn=1 ovr=3 disc=2C3 C=767 
009: fix=17E30EC3 : vr=1 btn=1 ser=7E30EC3 , enc=[99C1FFFD] --> dec=1EC30767 : btn=1 ovr=3 disc=2C3 C=767 
010: fix=17E30EC3 : vr=1 btn=1 ser=7E30EC3 , enc=[99C1FFFD] --> dec=1EC30767 : btn=1 ovr=3 disc=2C3 C=767 
011: fix=27E30EC3 : vr=0 btn=2 ser=7E30EC3 , enc=[03D99613] --> dec=2EC30768 : btn=2 ovr=3 disc=2C3 C=768 
012: fix=27E30EC3 : vr=1 btn=2 ser=7E30EC3 , enc=[03D99613] --> dec=2EC30768 : btn=2 ovr=3 disc=2C3 C=768 
013: fix=47E30EC3 : vr=0 btn=4 ser=7E30EC3 , enc=[2E5F68DF] --> dec=4EC30769 : btn=4 ovr=3 disc=2C3 C=769 
014: fix=87E30EC3 : vr=1 btn=8 ser=7E30EC3 , enc=[E81A3A01] --> dec=8EC3076A : btn=8 ovr=3 disc=2C3 C=76A 
015: fix=27E30EC3 : vr=0 btn=2 ser=7E30EC3 , enc=[A2D80E7C] --> dec=2EC3076B : btn=2 ovr=3 disc=2C3 C=76B 
016: fix=17E30EC3 : vr=0 btn=1 ser=7E30EC3 , enc=[F8FE34FC] --> dec=1EC3076C : btn=1 ovr=3 disc=2C3 C=76C 
017: fix=47E30EC3 : vr=0 btn=4 ser=7E30EC3 , enc=[3E3C92A0] --> dec=4EC3076D : btn=4 ovr=3 disc=2C3 C=76D 
018: fix=87E30EC3 : vr=0 btn=8 ser=7E30EC3 , enc=[06F256B0] --> dec=8EC3076E : btn=8 ovr=3 disc=2C3 C=76E 
019: fix=27E30EC3 : vr=0 btn=2 ser=7E30EC3 , enc=[58752313] --> dec=2EC3076F : btn=2 ovr=3 disc=2C3 C=76F 
020: fix=27E30EC3 : vr=0 btn=2 ser=7E30EC3 , enc=[553276E6] --> dec=2EC30770 : btn=2 ovr=3 disc=2C3 C=770 
```
