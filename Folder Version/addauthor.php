<?php
echo '<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>2750 A3: Add Author</title>
    <link rel="stylesheet prefetch" href="https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/3.3.7/css/bootstrap.min.css">
    <link rel="stylesheet" href="index.css">
  </head> 
  <body>';
      $programOutput = shell_exec("./a3 addauthor.wpml");
      echo $programOutput;
        $username = $_POST["username"];
        $streams = $_POST["streams"];
        $choice = $_POST["radio"];
        if ($choice == "Add" && $username != "" && $streams != "") {
          #echo "<h1>ADD: ./addauxthor -A \"$username\" \"$streams\"</h1>";
          exec("./addauthor -A \"$username\" \"$streams\"");
          #exec("./addauthor -A \"tes\" \"green\"");
          echo "<h3 style=\"text-align: center;\">Added $username to $streams</h3>";
          #$programOutput = shell_exec("./a3 addauthorfin.wpml");
        }
        if ($choice == "Remove" && $username != "" && $streams != "") {
          #echo "<h1>REMOVE: ./addauthor -A \"$username\" \"$streams\" -r</h1>";
          exec("./addauthor -A \"$username\" \"$streams\" -r");
          echo "<h3 style=\"text-align: center;\">Removed $username from $streams</h3>";
        }
echo '        
  </body>
</html>';
?>