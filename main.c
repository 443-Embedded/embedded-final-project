int milisecond = 1000;

void update() {
	int i;
	for(i = 0; i < milisecond * 12000; i++);
}

int main() {
	while(1) {
		update();
	}
}