<?php
//$time_start_ex = microtime(true);
define('SL',31);	//分割する桁数（とりあえず32ビットの正の数分=31桁）
stream_set_blocking(STDIN, 0);
$stdin = file_get_contents('php://stdin');
$first = strpos($stdin,"\n");
list($H,$W)=explode(' ',substr($stdin,0,$first));
$stdin = substr($stdin,$first+1);
$lines = explode("\n", $stdin);
$home_lines = array_slice($lines, 0, $H);
$widgets = array_slice($lines, $H + 1, $lines[$H]);
//$time_end = microtime(true);
//$time = ($time_end - $time_start_ex) * 1000;
//echo "!!!INPUT_TIME->$time!!!".PHP_EOL;
//print_r('!!!check!!!');
//print_r($home_lines);
//print_r($widgets);

if ($W > SL){
	//ホームの幅が広い場合はSL幅で計算
	$WW = SL;
}
else {
	$WW = $W;
}

// $bin_max_val = pow(2,$WW) - 1;		//2進数最大値	
// $str_max_val = str_repeat('1',$WW);	//文字列最大値
//$str_min_val = str_repeat('0',$WW);		//文字列最小値

//衝突チェックブロック配列
$check_block = array();
//スキマチェック用リスト
$check_list = array();
for ($SH=1;$SH<=$H;++$SH){
//	$check_block[$SH] = array();
	$check_list[$SH] = array();
}

//$SORT_COND = $H - 1;
foreach ($home_lines as $j => $hl_val){
	//echo "-----j->$j\n";
	//[最初の準備] ホームの情報をSLの幅で分けて2進数にし、配列に格納
	$check_block[$j] = array();
	$pos = 0;
	while ($pos<$W){
		$work_home = substr($hl_val,$pos,SL);
		if (strlen($work_home)<$WW){
			//最後のブロックの右側を1で埋める
			$work_home = str_replace(array("\r\n","\r","\n"), '', $work_home);
			$work_home = str_pad($work_home,$WW,'1');
		}
		$check_block[$j][] = intval($work_home, 2);		//2進数に変換して格納
		$pos += SL;
	}

	$str_check_list = '';
	foreach ($check_block[$j] as $i => $val){
	 	$str_bin = decbin($val);
	 	if (strlen($str_bin)<$WW){
	 		$str_bin = str_pad($str_bin, $WW, '0', STR_PAD_LEFT);
	 	}
	 	$str_check_list .= $str_bin;
	}

	$pos = 0;
	while ($pos<$W){
		$first = strpos($str_check_list,'0',$pos);
		if ($first !== false){
			$second = strpos($str_check_list,'1',$first);
			if ($second !== false){
				$key = $second - $first;
				$pos = $second;
			}
			else {
				$key = $W - $first;
				$pos = $W;
			}
			if (!isset($check_list[1][$key])){
				$check_list[1][$key]=1;
			}
			else {
				++$check_list[1][$key];
			}
		}
		else {
			break;
		}
	}
}
if (count($check_list[1])>1){
	krsort($check_list[1]);
}

for ($SH=2;$SH<=$H;++$SH){
	$k = $H - $SH;
	$cl = &$check_list[$SH];
	for ($j=0;$j<=$k;++$j){
		$str_check_list = '';
		$cb = &$check_block[$j];
		$cbp = &$check_block[$j+1];
		foreach ($cb as $i => $val){
			//OR演算
			$cb[$i] = $cbp[$i] | $val;
			// if ($check_block[$j][$i]===0){
			// 	$str_check_list .= $str_min_val;
			// }
			// else {
			 	$str_bin = decbin($cb[$i]);
			 	if (strlen($str_bin)<$WW){
			 		$str_bin = str_pad($str_bin, $WW, '0', STR_PAD_LEFT);
			 	}
			 	$str_check_list .= $str_bin;
			// }
		}

		$pos = 0;
		while ($pos<$W){
			$first = strpos($str_check_list,'0',$pos);
			if ($first !== false){
				$second = strpos($str_check_list,'1',$first);
				if ($second !== false){
					$key = $second - $first;
					$pos = $second;
				}
				else {
					$key = $W - $first;
					$pos = $W;
				}
				if (!isset($cl[$key])){
					$cl[$key]=1;
				}
				else {
					++$cl[$key];
				}
				//print_r($check_list[$SH]);
			}
			else {
				break;
			}
		}
	}
	if (count($cl)>1){
		krsort($cl);
	}
}

//print_r($check_list);
//$time_end = microtime(true);
//$time = ($time_end - $time_start_ex) * 1000;
//echo "!!!PREPARED_TIME->$time!!!".PHP_EOL;
$echo_str = '';
foreach ($widgets as $w_val){
	//ウィジェットループ
	list($SH,$TW) = explode(' ',$w_val);
	$cnt = 0;
	if ($SH>$H && $TW>$W){
		//ウィジェットのほうが大きい
	}
	else {
		foreach ($check_list[$SH] as $key => $val){
			if ($key<$TW) break;
			$cnt += ($key-$TW+1) * $val;
		}
	}
	//$echo_str .= "SH->$SH TW->$TW ".$cnt.PHP_EOL;
	$echo_str .= $cnt.PHP_EOL;
}
echo $echo_str;
//$time_end = microtime(true);
//$time = ($time_end - $time_start_ex) * 1000;
//echo "!!!TOTAL_TIME->$time!!!".PHP_EOL;
?>
