
== 2009-11-23 ==

php���� esctext2html ������������Щ���ܣ�

1. \p<eol> ���� \n<eol> תhtml֮ <BR> ��ǩ������<eol>Ϊ��ĩ��ǣ���'\r' �� '\n'��
2. \<Symbol> ת <a href="...">Symbol</a>������ href ��Ҫ��link base����ת����������Щ�ر�Ķ�����Ҫ����������ݿ��Լ� @see �Ĵ�����롣
3. html ��ǵ�����ʶ�𡣼����ı��г��� <TAG �� </TAG> ģʽʱ����Ҫ�����ж��Ƿ�Ϊ html ��ǡ�������ǣ�ת�� &lt;TAG��&lt;/TAG&gt;��������ԭ����

��TAGΪ��Щʱ����Ϊ��html��ǩ��
"a", "b", "i", "u", "img", "p", "br", "pre", "code", 				
"h1", "h2", "h3", "h4",	"li", "ul", "ol", "font",
"table", "tr", "th", "td", "em"

== 2009-11-22 ==

function makerel($href, $file) -> $rel

�ú��������� $file �ļ������� $href ����ļ�ʱ����Ҫ����������($rel)��
����˵������ $href �൱�� $file ��һ�����·����

���磬$href = "aaa/bbb/ccc/test.htm", $file = "aaa/ddd/e.htm"; �� makerel($href, $file) ���أ�"../bbb/ccc/test.htm"��
