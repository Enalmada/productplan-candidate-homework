# ProductPlan Candidate Homework

https://productplan-candidate-homework.now.sh/
(Takes a long time to spin up on the free hosting it is using)

Setup:
- devcert will ask for a certificate password to enable https in local dev mode.
Run the first `npm run dev` in a normal terminal...editor like Intellij may hide the one time password prompt from you.
` 

Install it and run:

```bash
npm install
npm run bsb-watch
(in another tab) npm run dev
```

Build and run:

```bash
npm run build
npm run start
```
(after running production build locally, make sure to unregister service worker in chrome dev tools)

## Technical highlights
* Server Side Rendering - Next.js provides superior performance over CRA https://medium.com/@steve_11957/nextjs-for-dummies-e7fa18719fe1
* Reason-React - cutting edge react. type safety, native reducers, etc: https://www.imaginarycloud.com/blog/reasonml-react-as-first-intended/
* Ant Design - designed for react (bootstrap and semantic design poorly maintained for react) 
* Font-Awesome - implemented without flicker and only loading icons used 
* localization ready - best practice to keep all your wording separate from the codebase
* non-critical css loaded async - see usage of loadCSS in _document.js 
* testing with jest - current principle of snapshot of whole page easily scaled to each component
* service worker - immutable assets like fonts cached better than native browser, push notification ready  (* production only)

Note this example uses a preexisting boilerplate I developed with other interesting production concepts to discuss:
https://github.com/Enalmada/next-reason-boilerplate

## Project Notes
* Apollo deprecates traditional redux and rest implementation giving apps more advanced features with less maintenance.  
It requires database setup so consider the usage of reason-react reducers holding state as a placeholder what Apollo would handle in its global cache. 
* CSS-in-JS is all the rage but I am still developing my personal opinion about which technology is "best".  Started using monolithic stylus for demo until I got 
around to cleaning it up for release but if we were going to stay with the stylus concept, it should be a file per component in production.  
* Font Awesome paid icons have a thin pro version matching specs better and would be used in production
* Gilroy font otf converted to web friendly version but it would be optimal to use a google served font for the browser
specific optimizations they provide.
* assets served locally out of /static directory for convenience but in production they would be in S3 and served from CDN
* drag button icons served with image for convenience but in production would be better to use a font

## Issues
* drag buttons allow you to to only grab the icon image.  This likely solved when custom icon prototype image placeholder is converted to font. 
* drag button conversion to bar position not precise because it is lacking the dashed border placeholder outline position in the drop area.    
* lane bar drag positioning is not accurate because I am looking at mouse location rather than bar element top left position.  I did drag and drop manually
to keep things simple for demo to avoid investing time on typesafe reason-react bindings for a drag and drop framework.  
A real project heavily based on drag and drop functionality would use an existing framework like react-beautiful-dnd.
* date lines and dots are totally "smoke and mirrors".  A real project would leverage existing technologies to power visual date ranges.

## Bonus Points
None of these are implemented in the current prototype but I think they are all low effort with consideration for each here:
* select bars and delete them - A custom right click menu might be a simple start but some people don't think about right clicking web stuff so 
I also propose a gear icon that shows up in top right corner on hover.  Both would enable the right click looking window. 
* application responsive - While I have used bootstrap "responsive first" media queries for most of my life, an application like this
deserves a more mobile focused alternative design like "ant design mobile" would provide.   
* bars drag & droppable into additional lanes within the timeline - this should just be a matter of updating the database with what lane contains what bar on drop 
underlying timeline horizontally scrollable - I feel like more investment in what should really power the timeline is necessary before this is worked on
Lane container expandable and collapsable - easily added by this common framework feature  https://ant.design/components/collapse/
API call / A/B testing - I believe using an existing service like google optimize (https://github.com/hudovisk/react-optimize) could be the best idea.


