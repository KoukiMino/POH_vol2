# time_start_ex = Time.now
SL = 31		#分割する桁数（とりあえず32ビットの正の数分=31桁）

h, w = gets.split(' ').map(&:to_i)
lines = $stdin.read().split(?\n).map(&:to_s)
home_lines = lines[0,h]
widgets = lines[h+1,lines[h].to_i]

# time_end = Time.now
# ptime = (time_end - time_start_ex) * 1000;
# puts '!!!STDIN_INPUT_TIME->!!!' << ptime.to_s() << "\n";

if w > SL
	#ホームの幅が広い場合はSL幅で計算
	ww = SL
else
	ww = w
end

bin_max_val = 2 ** ww -1
str_max_val = '1' * ww

#衝突チェックブロック用ハッシュ
check_block = Hash.new {|hash, key| hash[key] = Hash.new()}
#スキマチェック用ハッシュ
check_list = Hash.new {|hash, key| hash[key] = Hash.new()}

sort_cond = h - 1
count_max_sh = 0
next_max_sh = h
home_lines.each_with_index {|hl_val, j|
	#[最初の準備] ホームの情報をSLの幅で分けて2進数にし、配列に格納
	bin_home_list = Array.new
	pos = 0
	while pos < w do
		work_home = hl_val[pos,SL]
		if work_home.length < ww
			#最後のブロックの右側を1で埋める
			work_home = work_home.ljust(ww,'1')
		end
		bin_home_list.push(work_home.to_i(2))		#2進数に変換して格納
		pos += SL
	end

	#puts bin_home_list

	sh = j + 1
	while sh > 0 do
		if sh<=next_max_sh
			# [手順1] ウィジェットの高さパターンに応じてOR演算をしたリストを作成
			#(例)
			#     00100000
			#     01000011　⇒ 01100111
			#     01100100
			str_check_list = ''
			if sh>1
				sh_ck_block = check_block[sh-1]
			end
			sh_ck_block_new = check_block[sh]
			bin_home_list.each_with_index {|val, i|
				if sh==1
					chk = val
				else
					#OR演算
					if sh_ck_block[i]==bin_max_val
						chk = bin_max_val
					else
						chk = sh_ck_block[i] | val
					end
				end
				sh_ck_block_new[i] = chk
				#puts check_block

				#求めた2進数を文字列に変換
				if chk == bin_max_val
					str_check_list << str_max_val
				else
					wk_str = chk.to_s(2)
					if wk_str.length < ww
						wk_str = wk_str.rjust(ww,'0')
					end 
					str_check_list << wk_str
				end
			}

			#puts str_check_list

			# [手順2] 求めた2進数からゼロ文字列の長さリストを作成
			#(例)
			#     0011000110101011　⇒　2,3,1,1,1
			pos = 0
			sh_ck_list = check_list[sh]
			while pos < w do
				first = str_check_list.index('0',pos)
				if first.nil?
					if pos == 0
						#ゼロが存在しない　→　次のループより入りきるウィジェット高さが制限される
						next_max_sh = sh
					end
					break
				else
					second = str_check_list.index('1',first)
					if second.nil?
						key = w - first
						pos = w
					else
						key = second - first
						pos = second
					end
					if sh_ck_list.key?(key)
						sh_ck_list[key]+=1
					else
						sh_ck_list[key]=1
					end
				end
			end
			if j == sort_cond
				if sh_ck_list.length > 1
					check_list[sh] = sh_ck_list.sort_by{|key,val| -key}
					if sh>count_max_sh
						count_max_sh=sh
					end
				end
			end
			sh -= 1
		else
			sh = next_max_sh
		end
	end
}

#puts check_list

# time_end = Time.now
# ptime = (time_end - time_start_ex) * 1000;
# puts '!!!PREPARED_TIME->!!!' << ptime.to_s() << "\n";

puts_str = '';
for w_val in widgets do
	#ウィジェットループ
	sh, tw = w_val.split(' ').map(&:to_i)
	cnt = 0
	if sh <= count_max_sh
		check_list[sh].each{|key,value|
			break if key < tw
			cnt += (key - tw + 1) * value
		}
	end
	puts_str << cnt.to_s << "\n"
end
puts puts_str
# time_end = Time.now
# ptime = (time_end - time_start_ex) * 1000;
# puts '!!!TOTAL_TIME->!!!' << ptime.to_s() << "\n";
