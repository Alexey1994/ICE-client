ICE client compiling:<br>
<br>
Include <i>&lt;system/Windows.h&gt;</i> or <i>&lt;system/Linux.h&gt;</i> or <i>&lt;system/Android.h&gt;</i> in <i>main.c</i><br>
<br>
<pre>
Windows: gcc main.c -lpthread -lwsock32
Linux:   gcc main.c -lpthread
Android: gradlew build
</pre>
<br>
<br>
Signal server:<br>
<pre>
npm i
nodejs server.js
</pre>