# -*- coding: utf-8 -*-
# python 2.7.3
# import sys,time
import sys
# time_start_ex = time.time()

SL=31		#分割する桁数（とりあえず32ビットの正の数分=31桁）

# def decbin(bin,w):
#     if bin == 0:
# 		return '0' * w
#     bin_str = ''
#     i = w - 1
#     while i >= 0:
# 		bi = (bin >> i) & 1
# 		bin_str += str(bi)
# 		i -= 1
#     return bin_str

(H,W) = map(int, sys.stdin.readline().split(' '))
ALL = map(str, sys.stdin.read().splitlines())
home_lines = ALL[0:H]
N = int(ALL[H])
widgets = ALL[H+1:]
#print '!!!check!!!'
#print home_lines
#print widgets

# time_end = time.time()
# ptime = time_end - time_start_ex
# ptime = ptime * 1000
# ptime = str(ptime)
# print "!!!INPUT_TIME->" + ptime + "!!!"


if W > SL:
	WW = SL
else:
	WW = W

bin_max_val = pow(2,WW) - 1		#2進数最大値 
str_max_val = '1' * WW

#衝突チェックブロック配列
block_cnt = W // SL + 1
check_block = [[0 for j in range(block_cnt)] for i in range(H + 1)];

#スキマチェック用リスト
check_list = [{} for i in range(H + 1)];

#print check_block
#print check_list

SORT_COND = H - 1
for j,hl_val in enumerate(home_lines):

	#[最初の準備] ホームの情報をSLの幅で分けて2進数にし、配列に格納
	bin_home_list = []
	pos = 0
	append = bin_home_list.append
	while pos<W:
		work_home = hl_val[pos:pos+SL]
		if len(work_home)<WW:
			work_home = work_home.ljust(WW,'1')
		append(int(work_home,2))
		pos += SL

	SH = j + 1
	while SH > 0:
		# [手順1] ウィジェットの高さパターンに応じてOR演算をしたリストを作成
		#(例)
		#     00100000
		#     01000011　⇒ 01100111
		#     01100100
		#str_check_list = '';
		str_list = []
		append = str_list.append
		cblock = check_block[SH - 1]
		cblock_now = check_block[SH]
		for i,val in enumerate(bin_home_list):
			if SH==1:
				chk = val
			else:
				if cblock[i] == bin_max_val:
					chk = bin_max_val
				else:
					chk = cblock[i] | val
			cblock_now[i] = chk;

			#求めた2進数を文字列に変換
			if chk == bin_max_val:
				append(str_max_val)
			else:
				wk_str = bin(chk)
				wk_str = wk_str[2:].rjust(WW,'0')
				append(wk_str)

		# [手順2] 求めた2進数からゼロ文字列の長さリストを作成
		#(例)
		#     00100010101011　⇒　2,3,1,1,1
		str_check_list = "".join(str_list)
		clist = check_list[SH]
		pos = 0
		while pos < W:
			first = str_check_list.find('0',pos)
			if first >= 0:
				second = str_check_list.find('1',first)
				if second >= 0:
					key = second - first
					pos = second
				else:
					key = W - first
					pos = W
				if clist.has_key(key):
					clist[key] += 1
				else:
					clist[key] = 1
			else:
				break
		if j==SORT_COND:
			if len(clist) > 0:
			 	check_list[SH] = sorted(clist.items(),key=lambda x: x[0], reverse=True)
		SH -= 1

# time_end = time.time()
# ptime = time_end - time_start_ex
# ptime = ptime * 1000
# ptime = str(ptime)
# print "!!!PREPARED_TIME->" + ptime + "!!!"

#print check_list
#print_str = '';
print_list = ['0' for n in range(N)]
index=0
for index,w_val in enumerate(widgets):
	(SH,TW) = map(int, w_val.split(' '))
	cnt = 0
	if SH <= H:
		for key,val in check_list[SH]:
			if key < TW:
				break
			cnt += (key-TW+1) * val
		if cnt > 0:
			print_list[index] = str(cnt)
print "\n".join(print_list)
# time_end = time.time()
# ptime = time_end - time_start_ex
# ptime = ptime * 1000
# ptime = str(ptime)
# print "!!!TOTAL_TIME->" + ptime + "!!!"