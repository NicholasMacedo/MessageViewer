<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>2750 A3: Nicholas Macedo</title>
    <link rel="stylesheet prefetch" href="https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/3.3.7/css/bootstrap.min.css">
    <link rel="stylesheet" href="index.css">
  </head>
  <body>
    <?php
      #Creates the initial interface using index.wpml
      $programOutput = shell_exec("./a3 index.wpml");
      echo $programOutput;
    ?>
  </body>
</html>