<* This tests a variety of errors that should be caught by the extended CSS
 * checking functions introduced in V0.935
 *>

<html><head><title>CSS errors</title></head>
<body>
<* %p *>
<div style="font-size:15px"></div>
<div style="font-size:15"></div>
<div style="font-size:150%"></div>
<div style="font-size:15("></div>
<div style="font-size:15e"></div>
<div style="font-size:-15px"></div>

<* %P *>
<div style="text-indent:15px"></div>
<div style="text-indent:15"></div>
<div style="text-indent:150%"></div>
<div style="text-indent:15("></div>
<div style="text-indent:15e"></div>
<div style="text-indent:-15px"></div>

<* %n *>
<div style="word-spacing:15px"></div>
<div style="word-spacing:15"></div>
<div style="word-spacing:150%"></div>
<div style="word-spacing:15("></div>
<div style="word-spacing:15e"></div>
<div style="word-spacing:-15px"></div>

<* %c *>
<div style="background-color:black"></div>
<div style="background-color:foo"></div>
<div style="background-color:#fff"></div>
<div style="background-color:#ffff"></div>
<div style="background-color:#AAAAAA"></div>
<div style="background-color:rgb(1,2,3)"></div>
<div style="background-color:rgb(1,2)"></div>
<div style="background-color:rgb(100,200,300)"></div>
<div style="background-color:rgb(10%,200,30%)"></div>
<div style="background-color:rgb(1x,2a,3b)"></div>
<div style="background-color:RGB(10,20%,100%)"></div>
<div style="background-color:RGB(10,20%,120%)"></div>

<* %u *>
<p style="cue-before:url(:foo.au)"></p>
<p style="cue-before:url(a:/b.foo.au)"></p>
<p style="cue-before:url(a#:\b.foo.au)"></p>
<p style="cue-before:url(http://b.foo.au/)"></p>

<* %d *>
<p style="pitch-range: 50"></p>
<p style="pitch-range: 50x"></p>
<p style="pitch-range: a"></p>

<* %r *>
<p style="clip:auto"></p>
<p style="clip:rect(100px 10% 20em 3ex)"></p>
<p style="clip:rect(100px 10 20em 3ex)"></p>
<p style="clip:rect(100px,10% 20em 3ex)"></p>
<p style="clip:rect(10% 20em 3ex)"></p>
<p style="clip:rect(  100mm  1em 20ex 3%     )"></p>

</body>
</html>
