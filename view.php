<?php
 $servername = "localhost"; //服务器地址，默认为localhost
 $username = "lyy"; //用户名
  $password = "1010"; //密码
  $dbname = "lyy"; //数据库名称

   // parse_str( html_entity_decode( $_SERVER['QUERY_STRING']) , $out);

    $sqlq = "Select * from lyy ";

//      if ( array_key_exists( 'field' , $out ) ) {
//	          $sqlq = $sqlq . " where field = '" . $out['field'] . "'" ;
//		    }

      echo $sqlq;
      // 建立连接
       $conn = new mysqli($servername, $username, $password, $dbname);
       // 检查连接情况
       if ($conn->connect_error) {
           die("Connection failed: " . $conn->connect_error);
         }
       $result = mysqli_query( $conn , $sqlq );
       if ( $result->num_rows  > 0 ) {
    
	       echo "<table border='1'>
         <tr>
         <th>Data</th>
         <th>Time</th>
	 <th>Humidity</th>
	<th>Temperature</th>
	<th>Temperature2</th>
	<th>Pressure</th>
	<th>Light</th>
         </tr>";
       while($row = mysqli_fetch_array($result)) {
          echo "<tr>";
          echo "<td>" . $row['Data'] . "</td>";
          echo "<td>" . $row['Time'] . "</td>";
	  echo "<td>" . $row['Humidity'] . "</td>";
	  echo "<td>" . $row['Temperature'] . "</td>";
	  echo "<td>" . $row['Temperature2'] . "</td>";
	  echo "<td>" . $row['Pressure'] . "</td>";
	  echo "<td>" . $row['Light'] . "</td>";
          echo "</tr>";
       }
       echo "</table>";
       }
       $conn->close();
?>
