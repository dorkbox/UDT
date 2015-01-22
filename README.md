# Breaking the Data Transfer Bottleneck

UDT is a reliable UDP based application level data transport protocol for distributed data intensive applications
 over wide area high-speed networks. UDT uses UDP to transfer bulk data with its own reliability control and 
 congestion control mechanisms. The new protocol can transfer data at a much higher speed than TCP does. UDT
  is also a highly configurable framework that can accommodate various congestion control algorithms.  
  - Presentation: [PowerPoint](https://github.com/dorkbox/UDT/blob/master/udt-doc/udt-2009.ppt)
  - Poster: [PDF](https://github.com/dorkbox/UDT/blob/master/udt-doc/udt-sc08-poster.pdf)

### TCP 

TCP is [slow](http://barchart.github.com/barchart-udt/main/presentation-2009/img6.html).
UDT is [fast](http://barchart.github.com/barchart-udt/main/presentation-2009/img9.html).

### UDT

UDT is developed by [Yunhong Gu](http://www.linkedin.com/in/yunhong) and others at University of Illinois and Google.

UDT C++ implementation is available under [BSD license](http://udt.sourceforge.net/license.html)


### Barchart-UDT

Barchart-UDT is a Java wrapper around native C++ UDT protocol implementation.

Barchart-UDT is developed by Andrei Pozolotin and others at [Barchart, Inc.](http://www.barchart.com)

Barchart-UDT is available under [BSD license](http://udt.sourceforge.net/license.html)
as well.

Barchart-UDT exposes UDT protocol as both [java.net.Socket](http://java.sun.com/javase/6/docs/api/java/net/Socket.html) and [java.nio.channels.SocketChannel](http://java.sun.com/javase/6/docs/api/java/nio/channels/SocketChannel.html) and comes with a [java.nio.channels.spi.SelectorProvider](http://java.sun.com/javase/6/docs/api/java/nio/channels/spi/SelectorProvider.html)


### Key Features

**Fast**. UDT is designed for extremely high speed networks and it has been used to support global data transfer of terabyte sized data sets. UDT is the core technology in many commercial WAN acceleration products.

**Fair and Friendly**. Concurrent UDT flows can share the available bandwidth fairly, while UDT also leaves enough bandwidth for TCP.

**Easy to Use**. UDT resides completely at the application level. Users can simply download the software and start to use it. No kernel reconfiguration is needed. In addition, UDT's API is very similar to the traditional socket API so that existing applications can be easily modified.

**Highly Configurable**. UDT supports user defined congestion control algorithms with a simple configuration. Users may also modify UDT to suit various situations. This feature can also be used by students and researchers to investigate new control algorithms.

**Firewall Friendly**. UDT is completely based on UDP, which makes it easier to traverse the firewall. In addition, multiple UDT flows can share a single UDP port, thus a firewall can open only one UDP port for all UDT connections. UDT also supports rendezvous connection setup.


### Supported Platforms

Barchart-UDT is currently used on:

* Oracle JDK: 6, 7;
  
| ARCH/OS      |  Linux  | Mac OSX | Windows | 
|--------------|---------|---------|---------|
| arm-android  |   ???   |         |         |
| arm-rpi      |   +++   |         |         |
| x86/i386     |   YES   |   YES   |   YES   |
| x86-64/amd64 |   YES   |   YES   |   YES   |


### Current Implementation
 - Updates to UDT source 4.11 to fix some misc. CPU timing bugs in Linux (via the sourceforge help forum).
 - Cleaned up source for cross-compile environment in linux
 - Cleaned up preprocessor symbols and removed deprecated
 - Strips unneded symbols, drastically reducing size
 - Static linking to mingw libraries for windows build

