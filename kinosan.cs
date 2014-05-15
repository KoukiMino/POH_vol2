using System;
using System.Text;
using System.Runtime.InteropServices;

static class kinosan {
	[DllImport("libc")]
	static extern int read(int fd, IntPtr buf, int count);
	[DllImport("libc")]
	static extern int write(int fd, IntPtr buf, int count);

	static int read_int(byte[] pbuf, ref int pcount){
		int r=0;
		while('\n'!=pbuf[pcount] && ' '!=pbuf[pcount] && '\0'!=pbuf[pcount] ){
			r = r * 10 + pbuf[pcount++] - '0';
		}
		pcount++;
		return r;
	}

	static String buf_to_str(byte[] cbuf){
		StringBuilder sb = new StringBuilder();
		int length = cbuf.Length;
		int i=0;
		while(i<length){
			sb.Append((cbuf[i++] - '0'));
		}
		return sb.ToString();
	}

	static byte[] buf_or(byte[] buf1, byte[] buf2, int W){
		int i=0; 
		byte[] wbuf = new byte[W];
		while(i<W){
			if (buf1[i]==buf2[i]){
				wbuf[i] = buf1[i];
			}
			else {
				wbuf[i] = Convert.ToByte('1');
			}
			i++;
		}
		return wbuf;
	}

	static int get_pos_zero(byte[] cbuf, int pos, int W){
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

	static int get_pos_one(byte[] cbuf, int pos, int W){
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

	static void Main() {
		const int valueLength = 2 + 200000 + 300;
		var buffer = new byte[valueLength * 7];
		int count=0;
		var handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
		var pbuf = Marshal.UnsafeAddrOfPinnedArrayElement(buffer, 0);
		read(0, pbuf, valueLength * 7);

		int H = read_int(buffer,ref count);
		int W = read_int(buffer,ref count);
		//System.Console.WriteLine("H = {0} , W = {1}", H, W);
		int HP = H + 1;
		int WP = W + 1;

		byte[][] check_block = new byte[H][];
		int[][] check_list = new int[HP][];
		int[][] key_list = new int[HP][];
		for (int j=0;j<HP;j++){
			check_list[j] = new int[WP];
			key_list[j] = new int[WP];
		}

		int SH,TW,pos,first,second,key,cnt,index;

		for (int j=0;j<H;j++){
			//System.Console.WriteLine("-----j = {0}", j);
			check_block[j] = new byte[W];
			Buffer.BlockCopy(buffer, count, check_block[j], 0, W );
			//System.Console.WriteLine(buf_to_str(check_block[j]));
			count += W + 1;
			pos = 0;
			while (pos<W){
				first = get_pos_zero(check_block[j],pos,W);
				if (first>=0){
					second = get_pos_one(check_block[j],first,W);
					if (second >= 0){
						key = second - first;
						pos = second;
					}
					else {
						key = W - first;
						pos = W;
					}
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
			int k=H-SH;
			for (int j=0;j<=k;++j){
				check_block[j] = buf_or(check_block[j+1],check_block[j],W);
				pos = 0;
				while (pos<W){
					first = get_pos_zero(check_block[j],pos,W);
					if (first>=0){
						second = get_pos_one(check_block[j],first,W);
						if (second >= 0){
							key = second - first;
							pos = second;
						}
						else {
							key = W - first;
							pos = W;
						}
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

		//for (int j=0;j<H;j++){
		// 	for (int i=0;i<W;i++){
		// 		System.Console.WriteLine("SH = {0} TW = {1} {2}", (j+1), (i+1), check_list[j][i]);
		// 	}
		//}

		int N = read_int(buffer,ref count);
		int oi = 0;
		for (int i=0;i<N;i++){
			SH = read_int(buffer,ref count);
			TW = read_int(buffer,ref count);
			//System.Console.WriteLine("SH = {0} TW = {1}", SH, TW);
			cnt = 0;
			if (SH<=H && TW<=W){
				foreach (int k in key_list[SH]){
					if (k<TW) break;
					cnt += (k - TW + 1) * check_list[SH][k];
				}
			}
			if (10000 <= cnt)
				goto l10000;
			if (1000 <= cnt)
				goto l1000;
			if (100 <= cnt)
				goto l100;
			if (10 <= cnt)
				goto l10;
			goto l1;
			l10000:
				buffer[oi++] = (byte)('0' + Math.DivRem(cnt, 10000, out cnt));
			l1000:
				buffer[oi++] = (byte)('0' + Math.DivRem(cnt, 1000, out cnt));
			l100:
				buffer[oi++] = (byte)('0' + Math.DivRem(cnt, 100, out cnt));
			l10:
				buffer[oi++] = (byte)('0' + Math.DivRem(cnt, 10, out cnt));
			l1:
				buffer[oi++] = (byte)('0' + cnt);
				buffer[oi++] = 10;
		}
		write(1, pbuf, oi);
		handle.Free();
	}
}
