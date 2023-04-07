class Snake {
	constructor(size) {
		this.x = 0
		this.y = 0
		this.size = size
		this.div = [document.createElement('div')]

	}

	draw() {
		this.div[0].classList.add('snake')
		this.div[0].style.left = `${this.x}px`
		this.div[0].style.top = `${this.y}px`
		this.div[0].style.width = `${this.size}px`
		this.div[0].style.height = `${this.size}px`
		field.appendChild(this.div[0])
	}

	move(x, y) {
		this.x += x
		this.y += y
		if (this.x > fieldWidth - this.size || this.x < 0 || this.y > fieldHeight - this.size || this.y < 0) this.death()
		for (let i = this.div.length - 1; i > 0; i--) {
			this.div[i].style.left = this.div[i - 1].style.left
			this.div[i].style.top = this.div[i - 1].style.top
		}
		this.div[0].style.left = `${this.x}px`
		this.div[0].style.top = `${this.y}px`
		for (let i = 1; i < this.div.length; i++) {
			if (this.div[i].style.left === `${this.x}px` && this.div[i].style.top === `${this.y}px`) this.death()
		}
	}

	tailGrow() {
		const tail = document.createElement('div')
		tail.classList.add('snake')
		tail.style.width = `${this.size}px`
		tail.style.height = `${this.size}px`
		tail.style.left = `${this.x}px`
		tail.style.top = `${this.y}px`
		field.appendChild(tail)
		this.div.push(tail)
	}

	death() {
		this.x = 0
		this.y = 0
		xd = scale
		yd = 0
		if (highscore < count) {
		highscore = count
		socket.send(highscore)
	}
		count = 0
		for (let i = 1; i < this.div.length; i++) {
			field.removeChild(this.div[i])
		}
		this.div.splice(1, this.div.length - 1)
		counter()
	}
}
