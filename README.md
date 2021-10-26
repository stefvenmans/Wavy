# Wavy
Wavy was built using the open source application programming interface <a href="https://juce.com/">JUCE</a>. A recently developed C++ library that is capable of modelling arbitrary circuits using Wave Digital Filters [<a href="https://github.com/RT-WDF/rt-wdf_lib">RT-WDF</a>] was used and can be seen as the core of this application. Wavy makes it possible to build a circuit using a drag-and-drop interface and simulate it.
<img width="899" alt="wavy-screenshot" src="https://user-images.githubusercontent.com/15052685/130976660-404361d3-1ded-424b-8f0f-70a8ccdbc476.png">

<h3>How to build and run Wavy</h3>
<ol>
  <li>Create a new JUCE project using the Projucer. Make sure to select "Audio Application".</li>
  <li>Add the source code of this repository to the project just created.</li>
  <li>Add the <a href="https://github.com/RT-WDF/rt-wdf_lib">RT-WDF</a> library to the project folder. Make sure to install all libraries that it depends on. These are specified in the <a href="https://github.com/RT-WDF/rt-wdf_lib">RT-WDF</a> repository. </li>
  <li>You should now be able to build an executable and run it.</li>
  </ol> 
  
  <h3>Demo</h3>
  A demo of how Wavy can be used to model a T-bridged resonator can be found <a href="https://www.youtube.com/watch?v=t__uv63Sa9w">here</a>.
 
