<?php
echo '<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>2750 A3: View / Post Stream</title>
    <link rel="stylesheet prefetch" href="https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/3.3.7/css/bootstrap.min.css">
    <link rel="stylesheet" href="index.css">
  </head>
  <body>
  <h1>Stream Selection</h1>
  <div class="bigDiv">
  <h2>Please select a stream:</h2>
  <div class="center-block addAuthor">';
      $username = $_POST["username"];
      $programOutput = shell_exec("./getstreams.py \"$username\"");
      echo $programOutput;

      $cProgramOutput = shell_exec("./a3 getstream.wpml");
      echo $cProgramOutput;

echo '     
  </body>
</html>';
?>