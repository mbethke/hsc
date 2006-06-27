
while(<>) {
	if(/^DOCUMENT\s+[\da-f]+\s+\/.*/oi) {
		($num, $rest) = /DOCUMENT\s+([\da-f]+)\s+(.*)/oi;
		printf "DOCUMENT %x ..$rest",((hex $num)+2);
	} else {
		print;
	}
}
