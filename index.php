<?php
  $servername = "localhost"; //服务器地址，默认为localhost
  $username = "lyy"; //用户名
    $password = "1010"; //密码
    $dbname = "lyy"; //数据库名称

      $now = new DateTime();
      parse_str( html_entity_decode( $_SERVER['QUERY_STRING']) , $out);

        if ( array_key_exists( 'Humidity' , $out ) ) {
	  // 建立连接
		 $conn = new mysqli($servername, $username, $password, $dbname);
		     // 检查连接
		 if ($conn->connect_error) {
		     die("Connection failed: " . $conn->connect_error);
		 }
		
		 $datenow = $now->format('Y-m-d');
         echo '$datenow';
         $timenow = $now->format('H:i:s');
         echo '$timenow';
		 $Humi = $out['Humidity'];
		 $Temper = $out['Temperature'];
         $Temper2 = $out['Temperature2'];
         $P = $out['Pressure'];
         $L = $out['Light'];
		 $sql = "INSERT INTO lyy (Data, Time , Humidity  , Temperature ,Temperature2, Pressure, Light) VALUES ( '$datenow', '$timenow' , $Humi, $Temper, $Temper2, $P, $L)";
		
		 if ($conn->query($sql) === TRUE) {
		     echo "New record created successfully";
		 } else {
		     echo "Error: " . $sql . "<br>" . $conn->error;
		     }
		 $conn->close();
	} 
?>
