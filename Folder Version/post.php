<?php
echo '<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>2750 A3: Post to Stream</title>
    <link rel="stylesheet prefetch" href="https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/3.3.7/css/bootstrap.min.css">
    <link rel="stylesheet" href="index.css">
  </head>
  <body>';
      $username = $_POST['username'];
      $stream = $_POST["radio"];
      $postNum = $_POST["postNum"];

      echo "<h2> $username | Posting to Stream: $stream</h2>";

      #Calls a3 to run the config file. Uses post.wpml as the config file.
      exec("./a3 post.wpml",$programOutput,$ststus);
      foreach ($programOutput as $variable) {
        #Adds the hidden values to the forms when keyword is found in the form.
        if (strpos($variable, 'USERNAME') !== false) {
          $variable = strtr ($variable, array ('USERNAME' => "><input type=\"hidden\" name=\"username\" value=\"$username\""));
          $variable = strtr ($variable, array ('POSTNUM' => "><input type=\"hidden\" name=\"postNum\" value=\"$postNum\""));
          echo strtr ($variable, array ('STREAM' => "><input type=\"hidden\" name=\"radio\" value=\"$stream\""));
        } else{
          echo $variable;
        }
      }
      
      $programOutput = "";
      $postMessage = $_POST['postText'];
      if ($postMessage != "") {
          #Fixes the newlines and quotes from the input field.
          $postMessage = str_replace("\r\n","\n", $postMessage);
          $postMessage = str_replace("\"","\\\"", $postMessage);
          $postMessage = str_replace("'","\\'", $postMessage);
          $programOutput = shell_exec("./post -A \"$username\" \"$stream\" \"$postMessage\"");
          if ($programOutput != "") {
            echo "<h2>$programOutput</h2>";
          } else {
            echo "<h2>Something went wrong. :(</h2>";
          }
      }

echo '        
  </body>
</html>';
?>