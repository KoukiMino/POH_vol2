# use Data::Dumper;
# use Time::HiRes;  
# my $start_time = Time::HiRes::time;  
my $SL = 31;	#分割する桁数（とりあえず32ビットの正の数分=31桁）

<> =~ /^(\d+) (\d+)$/;
my $H = $1;
my $W = $2;
#print "H\n";
#print "$H\n";
#print "W\n";
#print "$W\n";

my @home_lines = map { scalar(<>) } (1..$H);
#@home_lines = map {$_ =~ s/\n//; $_} @home_lines;
map{scalar(<>)}(0);
#my @widgets = map { scalar(<>) } (0..$N-1);
#@widgets = map {$_ =~ s/\n//; $_} @widgets;

# printf("!!!INPUT_TIME->%0.3f\n",Time::HiRes::time - $start_time);

#print "home_lines\n";
#print @home_lines;

my $WW = $W;
if ($W > $SL){
	$WW = $SL;
}

my $bin_max_val = 2 ** $WW - 1;	#2進数最大値
my $str_max_val = '1' x $WW;		#文字列最大値

#print "$bin_max_val\n";
#print "$str_max_val\n";

#衝突チェックブロック配列
my @check_block;

#スキマチェック用ハッシュ
my %check_list;
keys(%check_list) = $H;

my $format1 = '%-0' . $WW . 's';
my $format2 = '%0' . $WW . 'b';
my $pos = 0;
my @bin_home_list;
my $str_check_list;
my $chk;
my $SH;
my $TW;

#my $COUNT_MAX_SH = 0;
my $NEXT_MAX_SH = $H;

foreach (0..$#home_lines) {
	#printf '[%d] %s' . "\n", $_, $home_lines[$_];

	#[最初の準備] ホームの情報をSLの幅で分けて2進数にし、配列に格納
	@bin_home_list = ();
	$pos = 0;
	while ($pos < $W) {
		my $work_home = substr($home_lines[$_], $pos, $SL);
		if (length($work_home)<$WW){
			#最後のブロックの右側を1で埋める
			$work_home = sprintf($format1, $work_home);
			$work_home=~ s/\s/1/g;
			#print "$work_home|\n";
		}
		#push @bin_home_list, oct("0b$work_home");
		$bin_home_list[@bin_home_list] = oct("0b$work_home");
		$pos += $SL;
	}

	#print Dumper @bin_home_list;

	$SH = $_ + 1;
	while ($SH > 0){
		if ($SH<=$NEXT_MAX_SH){
			# [手順1] ウィジェットの高さパターンに応じてOR演算をしたリストを作成
			#(例)
			#     00100000
			#     01000011　⇒ 01100111
			#     01100100
			$str_check_list = '';
			#$ck_block_ref = \%{$check_list{$SH-1}};
			foreach (0..$#bin_home_list) {
				if ($SH == 1){
					$chk = $bin_home_list[$_];
				}
				else {
					#OR演算
					if ($check_block{$SH-1}->{$_}==$bin_max_val) {
						$chk = $bin_max_val;
					}
					else {
						$chk = $check_block{$SH-1}->{$_} | $bin_home_list[$_];
					}
				}
				$check_block{$SH}{$_} = $chk;

				#求めた2進数を文字列に変換
				if ($chk == $bin_max_val){
					$str_check_list .= $str_max_val;
				}
				else {
					$str_check_list .= sprintf($format2,$chk);
				}

				#printf '[%d] %d %s' . "\n", $_, $SH, $str_check_list;
			}


			# [手順2] 求めた2進数からゼロ文字列の長さリストを作成
			#(例)
			#     0011000110101011　⇒　2,3,1,1,1
			$pos = 0;
			my $key;
			while ($pos < $W){
				my $first = index($str_check_list,'0',$pos);
				if ($first >= 0){
					my $second = index($str_check_list,'1',$first);
					if ($second >= 0){
						$key = $second - $first;
						$pos = $second;
					}
					else {
						$key = $W - $first;
						$pos = $W;
					}
					#if (exists($check_list{$SH}{$key})){
					 	$check_list{$SH}{$key}+=1;
					#}
					#else {
					#	$check_list{$SH}{$key}=1;
					#}
				}
				else {
					if ($pos==0){
						#ゼロが存在しない　→　次のループより入りきるウィジェット高さが制限される
						$NEXT_MAX_SH = $SH;
					}
					last;
				}
			}
			$SH -= 1;
		}
		else {
			$SH = $NEXT_MAX_SH;
		}
	}
	#print "-----block\n";
	#print Dumper @check_block;
}

#print "-----check_list\n";
#print Dumper %check_list;

# printf("!!!PREPARED_TIME->%0.3f\n",Time::HiRes::time - $start_time);

my $print_str = '';
my $cnt;
for my $w_val (<>) {
#foreach my $w_val (@widgets){
	#ウィジェットループ
	($SH,$TW) = split(/ /,$w_val);
	#$TW = chomp($TW);
	#print "SH->$SH\n";
	#print "TW->$TW\n";
	$cnt = 0;
	if ($SH<=$H){
		foreach (sort {$b <=> $a} (keys %{$check_list{$SH}}))  {
		#foreach (keys %{$check_list{$SH}})  {
			#printf '%d [%d] %d' . "\n", $SH, $_, $check_list{$SH}->{$_};
			if ($_ < $TW){
				last;
			}
			$cnt += ($_-$TW+1) * $check_list{$SH}->{$_};
		}
	}
	$print_str .= $cnt."\n";
}
print $print_str;
# printf("!!!TOTAL_TIME->%0.3f\n",Time::HiRes::time - $start_time);
