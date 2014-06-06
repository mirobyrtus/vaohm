
In this paper we present a realtime computer vision
approach to recognizing human movements based on pat
terns of motion The underlying representation is a
Motion History Image MHI which is characterized
by multiple histograms of the local motion orientations
The approach is adapted to accommodate movements of
dierent durations by using a simple iterative technique
Quantitative results are given showing discrimination
between dierent human movements using the approach
An extension addressing occlusion and distractor motion
is also presented within this framework

The recognition of human motion and action using
computer vision has widespread interest ranging from
surveillance applications to entertainment systems Be
ing able to recognize the presence of human motion is
desirable because every little change or movement in the
environment may not be consequential Monitoring ap
plications for instance may wish to signal only when
a person is seen in a particular area perhaps inside a
dangerous or secure area Thus only those motions be
longing to human activity are of importance In the
entertainment domain an increasing interest in peo
ple watching systems is growing Here the systems
watch for the gestures made by participants which con
troldrive the program or interaction 	

 Thus one
requirement or demand of such machine vision sys
tems is their ability to perform in realtime It would
not be of much use for the monitoring system to report
that a person entered into a dangerous area an hour
after the fact Also systems incorporating human ges
tures for input must recognize and respond quickly to
the user without noticeable lag to give a sense of im
mersion and actual control The quickness of response
is paramount
In this paper we present a realtime computer vision
approach to recognizing human movements In earlier
work 	
we described a representation of movement
known as a Motion History Image MHI The MHI is
a compact representation of temporal movement and is
simple to compute In this paper we present a new
method for recognizing movement which relies on local
ized regions of motion derived from the MHI By gath
ering and matching multiple overlapping histograms of
the motion orientations from the MHI we oer a real
time solution to recognizing various human movements
The remainder of this paper rst examines the re
lated research for which this work has been based Sec
tion 
Next we present the approach of using motion
histograms Section 
 This section is subdivided into
discussions on the underlying representation Section

 the calculation of the motion orientations Sec
tion 

and the histogram generation Section 


We then present a simple recognition method Section
along with some quantitative results Section
A
method for handling variablelength movements is also
described Section 
 We then address the notions of
occlusion and distractor motions within an extension of
this framework Section  Lastly we conclude with a
brief summary of the approach Section

In this paper we presented a realtime computer vision
approach to recognizing human movements based on
patterns of motion The underlying representation is a
Motion History Image MHI which is characterized by
multiple overlapping histograms of the motion orienta
tions These histograms separate and localize regions of
motion for a better description of the movement Quan
titative results show that the method can easily discrim
inate between dierent human movements and is ex
tendible to variable length motions Occlusion and dis
tractor motions are also addressable within this frame
work
