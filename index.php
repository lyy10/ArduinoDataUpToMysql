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
		
		 $datenow = $now->format('Y-m-d H:i:s');
		 $Humi = $out['Humidity'];
		 $Temper = $out['Temperature'];
		 $sql = "INSERT INTO lyy ( Time , Humidity  , Temperature) VALUES ( '$datenow' , $Humi, $Temper )";
		
		 if ($conn->query($sql) === TRUE) {
		     echo "New record created successfully";
		 } else {
		     echo "Error: " . $sql . "<br>" . $conn->error;
		     }
		 $conn->close();
	} 
?>
