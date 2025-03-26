# Results
 - The AI generated descriptions with comments generally produced a higher score relative to the reference text, but by a very small margin compared to the code description generated without comments. 

 - When manually reading the texts, both me and Felix generally prefer the description generated without comments due to it being more easily readable, and had more of a "flowy" style to it, not jumping to much between segments. The code descriptions generated with comments are however more detailed when it comes to functionality, so theres a tradeoff. 

- The ai descriptions genereated with and without code are generally very similar in score and writing style, indicating that the output is generally very similar whether comments in the code are included or not. 

- Deepseek consistently ignored the limitations imposed upon it through the prompt and produced outputs that were much longer than the word limit we specified. This led to us having to remove the Deepseek model from the experiment due to this not being a fair evaluation if one model is able to utilize a larger token length. The token length was imposed due to SBERT having a token length of 250. There are other models that have larger token lenths but these would require fine tuning and model training which would be out of scope in this project, might be more applicable for a masters thesis. 

- According to specifications by our supervisors, the code description was meant to be high level and thus this was included within the prompt. The code description generated with comments generally went more in depth than the code description generated without comments.

- The AI model struggled with producing code descriptions under the word limitation imposed upon it. When presented with larger code bases such as the quantization library and the multichannel library, it took several tries before the models eventually produced a more "high-level" description which thus made the output stay within the token length. 
