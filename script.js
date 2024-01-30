document.querySelectorAll('.nav-item').forEach(item => {
  item.addEventListener('click', function(e) {
    e.preventDefault(); // Prevent the default anchor link behavior

    const color = this.getAttribute('data-color'); // Get the color from the clicked item
    const url = this.getAttribute('href'); // Get the URL to open
    localStorage.setItem('topBarColor', color); // Save the color to localStorage

    // Change the top bar color
    document.querySelector('.top-bar').style.backgroundColor = color;

    // Open the new page in the same tab
    window.location.href = url;

    // If you prefer to open the new page in a new tab, comment out the above line and use:
    // window.open(url, '_blank');
  });
});

document.addEventListener('DOMContentLoaded', function() {
  const savedColor = localStorage.getItem('topBarColor');
  if (savedColor) {
    document.querySelector('.top-bar').style.backgroundColor = savedColor;
  }
});

document.querySelector('.logo').addEventListener('click', function() {
  localStorage.removeItem('topBarColor'); // Option 1: Clear the stored color
  // Or Option 2: Reset the color to white
  // localStorage.setItem('topBarColor', 'white');
});

document.querySelector('.about').addEventListener('click', function() {
  localStorage.removeItem('topBarColor'); // Option 1: Clear the stored color
  // Or Option 2: Reset the color to white
  // localStorage.setItem('topBarColor', 'white');
});
