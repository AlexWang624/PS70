// do not deploy this API Key or upload onto Github
const OPENAI_API_KEY = ***;
const submitButton = document.querySelector('#submit');
const outPutElement = document.querySelector('#output');
const historyElement = document.querySelector('.history');
const buttonElement = document.querySelector('button');
const textInputContainer = document.getElementById('textInputContainer');
const inputElement = textInputContainer.querySelector('input');
var textOverImages = document.querySelectorAll(".onClickTextOverImage");
var previousTextOverImage;
var clickCount = 0;


let conversationHistory = [{
    "role": "system",
    "content": "You are a creative novelist. You write experimental psychological thrillers that are based on the Major Arcana Tarot cards with horror and supernatural elements. Your stories have unexpected plot twists that keep the audience engaged and the settings of the stories are culturally ambiguous. Think outside the box and be creative."
}
];

// Function to handle adding messages to the conversation history
function addToConversation(role, content) {
    conversationHistory.push({ role, content });
}

// Click Tarot Cards
for (var i = 0; i < textOverImages.length; i++) {
    // When a card is on click, show the name of the card
    textOverImages[i].onclick = function () {
        var classes = this.classList;
        var title = this.querySelector("div").textContent;
        if (classes.contains("show")) {
            classes.remove("show");
        }
        else {
            if (previousTextOverImage != null)
                previousTextOverImage.classList.remove("show");
            previousTextOverImage = this;
            classes.add("show");
        }

        // Begin the story with the first click, and continue the story with sequential clicks
        var displayText;
        if (clickCount === 0) {
            // First click shows the title
            // displayText = "Begin a story with " + this.querySelector("div").textContent + " on the first position";
            displayText = "Begin a story with " + this.querySelector("div").textContent;
        }
        else if (clickCount === 9) {
            // Subsequent clicks show different text
            // displayText = "End the story with " + this.querySelector("div").textContent + " on position number " + clickCount;
            displayText = "Based on the story you just wrote, end the story with " + this.querySelector("div").textContent;
        }
        else {
            // displayText = "Continue the story with " + this.querySelector("div").textContent + " on position number " + clickCount;
            displayText = "Based on the story you just wrote, continue the story with " + this.querySelector("div").textContent;
        }

        // When the number of clicks is greater than or equal 10, clickCount returns to 0
        if (clickCount >= 9) {
            clickCount = 0;
        }

        // Display the name of the card as a text input
        textInputContainer.innerHTML = '';
        var input = document.createElement("input");
        input.type = "text";
        input.value = displayText;
        textInputContainer.appendChild(input);
    }
}

// Typing Output
function typeOutText(element, text, typingSpeed) {
    let i = 0;
    const interval = setInterval(() => {
        if (i < text.length) {
            element.textContent += text.charAt(i);
            i++;
        } else {
            clearInterval(interval);
        }
    }, typingSpeed);
}

// GPT API

async function getMessage() {
    console.log('clicked');
    clickCount++;
    const inputElement = document.querySelector('#textInputContainer input');
    const inputValue = inputElement.value;

    addToConversation('user', inputValue);

    const options = {
        method: 'Post',
        headers: {
            'Authorization': `Bearer ${OPENAI_API_KEY}`,
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            model: "ft:gpt-3.5-turbo-0613:personal::8WtrYSYG",
            messages: conversationHistory,
          temperature: 1,
          max_tokens: 256,
          top_p: 1,
          frequency_penalty: 0,
          presence_penalty: 0
        })
    };
    console.log('inputValue: ' + conversationHistory);

    try {
        const response = await fetch('https://api.openai.com/v1/chat/completions', options)
        const data = await response.json()
        // console.log(data)

        // Create a new paragraph element for the new response
        const newResponse = document.createElement('p');
        // newResponse.textContent = data.choices[0].message.content;

        // Append the new response paragraph to the output element
        outPutElement.appendChild(newResponse);

         // Use the typeOutText function to display the response
         const storyText = data.choices[0].message.content;
         typeOutText(newResponse, storyText, 10);


        addToConversation('assistant', storyText);
        // outPutElement.textContent = data.choices[0].message.content

        if (data.choices[0].message.content && inputElement.value) {
            const pElement =  document.createElement('p')
            pElement.textContent = inputElement.value
            pElement.addEventListener('click', () => changeInput(pElement.textContent))
            historyElement.append(pElement)
        }
    } catch (error){
        console.error(error)

    }
}

submitButton.addEventListener('click', getMessage)
