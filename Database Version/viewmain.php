<?php
#Draws the head of the document.
echo '<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>2750 A3: View Stream</title>
    <link rel="stylesheet prefetch" href="https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/3.3.7/css/bootstrap.min.css">
    <link rel="stylesheet" href="index.css">
  </head>
  <body>';
      $username = $_POST['username'];
      $stream = $_POST["radio"];
      $postNum = $_POST["postNum"];
      $sortNum = $_POST["sortNum"];
      $totalNumMessages = shell_exec("./dbFunctions -getNumMessages \"$stream\"");

      #If the postNum is -1 then I need to get the information of how many posts read from the file.
      if ($postNum == "-1") {
        $postNum = shell_exec("./getNumRead.py \"$username\" \"$stream\"");
      }

      #Previous is pushed, check to see if sorting or not and act accordingly. Also ensure that number is not going past zero.
      if (isset($_POST["Previous"])) {
        if ($sortNum != "" && $sortNum != "-1") {
          if ($sortNum != "0") {
            $sortNum = $sortNum - 1;
          }
        } else {
          if ($postNum != 0) {
            $postNum = $postNum - 1;
          }
        }
      }

      #Next is pushed, check to see if sorting or not and act accordingly. make sure not going past number of messages in file.
      if (isset($_POST["Next"])) {
        if ($sortNum != "" && $sortNum != "-1") {
          if ($sortNum < $totalNumMessages) {
            $sortNum = $sortNum + 1;
          }
        } else {
          if ($postNum < $totalNumMessages) {
            $postNum = $postNum + 1;
          }
        }
      }

      #Sortis pushed, check to see if sorting or not and toggle on or off.
      if (isset($_POST["Sort"])) {
        if ($sortNum == "" || $sortNum == "-1") {
          $sortNum = "0";
        } else {
          $sortNum = "-1";
        }
      }

      #Sets the valye for the sort to -1 if ununitilized.
      if ($sortNum == "" && $sortNum != "0") {
        $sortNum = "-1";
      }

      #Calls the python program to mark all as read for the given stream and user.
      if (isset($_POST["Mark_All_Read"])) {
        exec("./markAsRead.py \"$username\" \"$stream\" \"$totalNumMessages\"");
      }

      echo "<h2> Viewing $stream as $username </h2>";

      #Exec the call to A3 to draw the screen. USES viewmain.wpml
      exec("./a3 viewmain.wpml",$programOutput,$ststus);
      foreach ($programOutput as $variable) {
        #Adds the hidden values to the forms when keyword is found in the form.
        if (strpos($variable, 'USERNAME') !== false) {
          $variable = strtr ($variable, array ('USERNAME' => "><input type=\"hidden\" name=\"username\" value=\"$username\""));
          $variable = strtr ($variable, array ('POSTNUM' => "><input type=\"hidden\" name=\"postNum\" value=\"$postNum\""));
          $variable = strtr ($variable, array ('SORTNUM' => "><input type=\"hidden\" name=\"sortNum\" value=\"$sortNum\""));
          echo strtr ($variable, array ('STREAM' => "><input type=\"hidden\" name=\"radio\"value=\"$stream\""));
        } else if (strpos($variable, 'EXECVIEWPROGRAMHERE') !== false) {
          #When the above phrase is found in the config file, the following code is executed to produce the text area with the information inside it.
          if ($sortNum == "-1") {
            $streamInfo = shell_exec("./getNewMessage.py \"$username\" \"$stream\" \"$postNum\"");
            echo "<textarea rows=\"15\" cols=\"80\" readonly>$streamInfo</textarea>";
            $postDispNum = $postNum + 1;
            echo "<br><p>Sort: Off | Saving Last Read: On | Message: $postDispNum/$totalNumMessages </p>";
          } else { #Runs if the sort button IS pushed.
            $streamInfo = shell_exec("./getNewSortMessage.py \"$username\" \"$stream\" \"$sortNum\"");
            echo "<textarea rows=\"15\" cols=\"80\" readonly>$streamInfo</textarea>";
            $sortDispNum = $sortNum + 1;
            echo "<br><p>Sort: On | Saving Last Read: Off | Message: $sortDispNum/$totalNumMessages</p>";
          }
        }else{
          echo $variable;
        }
      }
echo '        
  </body>
</html>';
?>