forky: forky.c
	gcc -o forky forky.c

results: forky
	(echo "Pattern 1:"; ./forky 1 ${shell bc --expression="$$RANDOM % 15 + 5"}; \
	echo "Pattern 2:"; ./forky 2 ${shell bc --expression="$$RANDOM % 15 + 5"}; \
	echo "Pattern 3:"; ./forky 3 ${shell bc --expression="$$RANDOM % 15 + 5"}) \
		> results.txt

clean:
	rm -f forkymake results
