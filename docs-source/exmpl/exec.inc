<$EXEC COMMAND="list >t:hugo.list DIRS">   <* write listing to "t:hugo.list" *>
<$INCLUDE FILE="t:hugo.list" SOURCE PRE>   <* include directory list *>
<$EXEC COMMAND="delete t:hugo.list QUIET"> <* remove temporary file *>

