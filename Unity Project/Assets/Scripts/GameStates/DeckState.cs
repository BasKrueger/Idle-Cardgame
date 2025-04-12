using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class DeckState 
{
    public int currentCard;
    public List<CardState> deckContent;

    private List<CardState> InPlayOrder_;
    public List<CardState> InPlayOrder
    {
        get
        {
            if (deckContent == null)
            {
                return new List<CardState>();
            }

            if (InPlayOrder_ == null)
            {
                InPlayOrder_ = new List<CardState>();
                for (int i = currentCard; InPlayOrder_.Count != deckContent.Count; i++)
                {
                    if (i >= deckContent.Count)
                    {
                        i = 0;
                    }

                    InPlayOrder_.Add(deckContent[i]);
                }
            }

            return InPlayOrder_;
        }
    }
}
