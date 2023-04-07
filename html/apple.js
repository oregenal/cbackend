class Apple {
	constructor(size) {
		this.size = size
		this.div = document.createElement('div')
	}

	randomLocation() {
		this.x = Math.floor(Math.random() * (fieldWidth - this.size) / this.size) * this.size
		this.y = Math.floor(Math.random() * (fieldHeight - this.size) / this.size) * this.size
	}

	draw() {
		this.div.style.backgroundColor = 'red'
		this.div.style.width = `${this.size}px`
		this.div.style.height = `${this.size}px`
		this.div.style.borderRadius = `30%`
		this.div.style.position = 'absolute'
		this.randomLocation()
		this.div.style.top = `${this.y}px`
		this.div.style.left = `${this.x}px`
		field.appendChild(this.div)
	}

	eaten(snake) {
		if (this.x === snake.x && this.y === snake.y) {
			let chk = false
			do {
				this.randomLocation()
				for (let i = 0; i < snake.div.length; i++) {
					if (snake.div[i].style.left === `${this.x}px` && snake.div[i].style.top === `${this.y}px`) {
						chk = true
						break
					} else {
						chk = false
					}
				}
			} while (chk)
			this.div.style.top = `${this.y}px`
			this.div.style.left = `${this.x}px`
			snake.tailGrow()
			count++
			counter()
		}
	}
}
