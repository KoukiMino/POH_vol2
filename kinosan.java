public class kinosan {
	static final int SIZE=8*(2+200000+300);
	static byte[] buf=new byte[SIZE];
	static int count=0;
	static int W;

	static int read_int(){
		int r=0;
		while('\n'!=buf[count] && ' '!=buf[count] && '\0'!=buf[count] ){
			r = r * 10 + buf[count++] - '0';
		}
		count++;
		return r;
	}

	static byte[] read_buf(){
		byte[] wbuf = new byte[W];
		int i=0; 
		while('\n'!=buf[count] && ' '!=buf[count] && '\0'!=buf[count] ){
			wbuf[i++] = buf[count++];
		}
		count++;
		return wbuf;
	}

	// static String buf_to_str(byte[] cbuf){
	// 	StringBuffer sbuf = new StringBuffer();
	// 	int length = cbuf.length;
	// 	int i=0;
	// 	while(i<length){
	// 		sbuf.append((cbuf[i++] - '0'));
	// 	}
	// 	return sbuf.toString();
	// }

	static byte[] buf_or(byte[] buf1, byte[] buf2){
		int i=0; 
		byte[] wbuf = new byte[W];
		while(i<W){
			if (buf1[i]==buf2[i]){
				wbuf[i] = buf1[i];
			}
			else {
				wbuf[i] = '1';
			}
			i++;
		}
		return wbuf;
	}

	static int get_pos_zero(byte[] cbuf, int pos){
		int ret = -1;
		while(pos<W){
			if (cbuf[pos] == '0'){
				ret = pos;
				break;
			}
			pos++;
		}
		return ret;
	}

	static int get_pos_one(byte[] cbuf, int pos){
		int ret = -1;
		while(pos<W){
			if (cbuf[pos] == '1'){
				ret = pos;
				break;
			}
			pos++;
		}
		return ret;
	}

	public static void main(String[] args){
		try{
			// long start = System.currentTimeMillis();
			// long stop;
			int buf_size = System.in.read(buf,0,SIZE);

			// stop = System.currentTimeMillis();
			// System.out.println("!!!INPUT_TIME->" + (stop - start));

			int H,SH,TW,pos,first,second,key,cnt,index;
			H = read_int();
			W = read_int();
			int HP = H + 1;
			int WP = W + 1;
			// System.out.println("H->" + H);
			// System.out.println("W->" + W);

			byte[][] check_block = new byte[H][W];
			int check_list[][] = new int[HP][WP];
			int key_list[][] = new int[HP][WP];

			for(int j=0;j<H;j++){
				// System.out.println("!!!-----j->" + j);

				check_block[j] = new byte[W];
				// bin_home_list = read_buf();
				System.arraycopy(buf, count, check_block[j], 0, W);
				count += W + 1;
				// System.out.println(buf_to_str(bin_home_list));
				pos = 0;
				while (pos<W){
					// System.out.println("!!!!!-----pos->" + pos);
					first = get_pos_zero(check_block[j],pos);
					// System.out.println("first->" + first);
					if (first >= 0){
						second = get_pos_one(check_block[j],first);
						// System.out.println("second->" + second);
						if (second >= 0){
							key = second - first;
							pos = second;
						}
						else {
							key = W - first;
							pos = W;
						}
						// System.out.println("SH->" + SH);
						// System.out.println("key->" + key);
						++check_list[1][key];
					}
					else {
						break;
					}
				}
			}
			key = W;
			index = 0;
			while (key>0){
				if (check_list[1][key]>0){
					key_list[1][index++]=key;
				}
				--key;
			}

			for (SH=2;SH<=H;++SH){
				int k = H - SH;
				for (int j=0;j<=k;++j){
					check_block[j] = buf_or(check_block[j+1],check_block[j]);
					pos = 0;
					while (pos<W){
						// System.out.println("!!!!!-----pos->" + pos);
						first = get_pos_zero(check_block[j],pos);
						// System.out.println("first->" + first);
						if (first >= 0){
							second = get_pos_one(check_block[j],first);
							// System.out.println("second->" + second);
							if (second >= 0){
								key = second - first;
								pos = second;
							}
							else {
								key = W - first;
								pos = W;
							}
							// System.out.println("SH->" + SH);
							// System.out.println("key->" + key);
							++check_list[SH][key];
						}
						else {
							break;
						}
					}
				}
				key = W;
				index = 0;
				while (key>0){
					if (check_list[SH][key]>0){
						key_list[SH][index++]=key;
					}
					--key;
				}
			}			

			// stop = System.currentTimeMillis();
			// System.out.println("!!!PREPARED_TIME->" + (stop - start));

			// for (int j=0;j<H;j++){
			//  	for (int i=0;i<W;i++){
			//  		System.out.println("SH->" + (j+1) + " TW->" + (i+1) + ' ' + check_list[j][i]);
			//  	}
			// }

			int N = read_int();
			// System.out.println("N->" + N);
			StringBuffer sbuf = new StringBuffer();
			for (int i=0;i<N;i++){
				SH = read_int();
				TW = read_int();
				// System.out.println("SH->" + SH);
				// System.out.println("TW->" + TW);
				cnt = 0;
				if (SH<=H && TW<=W){
					for (int kk: key_list[SH]){
						if (kk<TW) break;
						cnt += (kk - TW + 1) * check_list[SH][kk];
					}
				}
				sbuf.append(cnt);
				sbuf.append("\n");
			}
			System.out.print(sbuf.toString());
			// stop = System.currentTimeMillis();
			// System.out.println("!!!TOTAL_TIME->" + (stop - start));
		}catch(Exception e){
			System.err.println(e);
		}
	}
}