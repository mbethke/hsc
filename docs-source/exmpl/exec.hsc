<$if COND=(HSC.SYSTEM="AMIGA")>
  <$exec COMMAND="list DIRS" TEMPORARY INCLUDE SOURCE PRE>
<$elseif COND=(HSC.SYSTEM="UNIX")>
  <$exec COMMAND="ls -ld" TEMPORARY INCLUDE SOURCE PRE>
<$else>
  <$message text="operating system not supported" class="warning">
</$if>
